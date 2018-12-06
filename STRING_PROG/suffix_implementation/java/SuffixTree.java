/*
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL,
 * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY
 * THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 *
 * @author Bogdan Dorohonceanu
 */

import java.util.*;

public class SuffixTree extends SuffixArray {
    
    private final static int MEGA = 1024 * 1024;    
    // prefix length between current suffix and next suffix in the ordered suffixes
    public int maxPrefix = 0;
    public double avgPrefix = 0;
    // tree root
    private int root_;
    // number of nodes, excluding the root
    private int nodes_ = -1;
    // a node consists in: index of substring in suffix, 
    // first node child/suffix, and node sibling
    private int[] index_;
    private int[] child_;
    private int[] sibling_;
    // node size = 3 integers of 4 bytes each = 12 bytes
    private final static int NODE_SIZE = 3 * 4;
    // resizing increment for node array capacity
    private static int increment_;
    // were the the node arrays resized?
    private boolean wasResized_ = false;
    // null link to a node
    private final static int NULL = 0;
    
    // Constructs a suffix tree.
    public SuffixTree(int capacity, int increment, Sequence[] s, boolean show) {
        super(s);
        increment_ = (increment > 4) ? increment : 4;
        if (show) System.err.println(super.toString());
        index_ = new int[capacity];
        child_ = new int[capacity];
        sibling_ = new int[capacity];
        Watch w = new Watch();
        w.start();
        // compute common prefix lengths for suffixes
        int prefix = 0, first, last, tmp, tmp2;
        first = pos[0];
        last = offset_[findSequenceNo(first) + 1] - 1; /*ignore EOS*/
        // root must be the first node and have index 0
        root_ = newNode(0, NULL, NULL);
        tmp = newNode(last, -pos[0], NULL);
        tmp = newNode(first, tmp, NULL);
        child_[root_] = tmp;
        if (show) System.err.println(toString());
        for (int i = 1; i < length_; i++) {
            // compute prefix
            int i1 = pos[i - 1], i2 = pos[i];
            while ((bytes_[i1] == bytes_[i2]) && (bytes_[i1] != EOS)) {
                i1 ++; 
                i2 ++;
            }
            prefix = i2 - pos[i];
            if (maxPrefix < prefix) {
                maxPrefix = prefix;
            }
            avgPrefix += prefix;            
            // insert suffix
            first = pos[i];
            last = offset_[findSequenceNo(first) + 1] - 1; /*ignore EOS*/
            if (prefix == 0) {
                tmp = newNode(last, -pos[i], NULL);
                tmp = newNode(first, tmp, child_[root_]);
                child_[root_] = tmp;
            }
            else {
                int node = child_[root_];
                int nodeLength = index_[child_[node]] - index_[node];
                while (prefix > nodeLength) { 
                    index_[node] = first;
                    first += nodeLength;
                    prefix -= nodeLength;
                    node = child_[node];
                    nodeLength = (index_[child_[node]] - index_[node]);
                }
                if (prefix == nodeLength) {
                    index_[node] = first;
                    first += prefix;
                    if (first == last) {
                        tmp = newNode(first, -pos[i], child_[node]);
                        child_[node] = tmp;
                    }
                    else {
                        tmp = newNode(last, -pos[i], NULL);
                        tmp = newNode(first, tmp, child_[node]);
                        child_[node] = tmp;
                    }
                }
                // prefix < nodeLength
                else { 
                    int index = index_[node] + prefix;
                    index_[node] = first;
                    first += prefix;
                    // split the node
                    if (first == last) {
                        tmp = newNode(index, child_[node], NULL);
                        tmp = newNode(first, -pos[i], tmp);
                        child_[node] = tmp;
                    }
                    else {
                        tmp = newNode(last, -pos[i], NULL);
                        tmp2 = newNode(index, child_[node], NULL);
                        tmp = newNode(first, tmp, tmp2);
                        child_[node] = tmp;
                    }
                }
            }
            if (show) System.err.println(toString());
        }
        // free some memory
        pos = null;
        System.gc();
        int n = nodes_ + 1;
        // print statistics 
        System.err.println("SuffixTree built from SuffixArray in " + w.msecs() + " msec.");
        avgPrefix = Math.floor((avgPrefix / length_) * 100.0 + 0.5) / 100.0;
        System.err.println("- maxPrefix " + maxPrefix + ", avgPrefix " + avgPrefix);
        System.err.println("- " + (n - length_) + " (inner) nodes, " + length_ + " sufixes (leaves)");
        int d = (int)Math.floor(0.5 + (1.0 * n * NODE_SIZE) / MEGA);
        System.err.println("- " + n + " nodes, represented on " + d + " MB");
        if (wasResized_ || (capacity > n)) {
            setCapacity(n);
        }
        if (show) System.err.println("\nFinal tree:\n\n" + toString());
    }
    
    private Vector v_ = null;
    private byte[] sBytes_ = null;
    
    // Search a String into the tree. 
    public Result[] search(String s) {
        v_ = new Vector();
        sBytes_ = s.getBytes();
        for (int child = child_[root_]; child > 0; child = sibling_[child]) {
            search(0, child);
        }
        sBytes_ = null;
        Result[] r = new Result[v_.size()];
        for (int i = 0; i < r.length; i++) {
            r[i] = (Result)v_.elementAt(i);
        }
        v_ = null;
        return r;
    }
    
    // Auxiliary method for searching a String into the tree
    // by traversing the tree.
    private void search(int start, int node) {
        // inner node
        if (child_[node] > 0) {
            for (int index = index_[node], last = index_[child_[node]]; 
                (start < sBytes_.length) && (index < last); start ++, index ++) 
            {
                if (sBytes_[start] != bytes_[index]) {
                    return;
                }
            }
            for (int child = child_[node]; child > 0; child = sibling_[child]) {
                search(start, child);
            }
        }
        // leaf
        else {
            int suffix = -child_[node];
            int seqNo = findSequenceNo(suffix);
            int seqSuffix = suffix - offset_[seqNo];
            v_.add(new Result(seqNo, seqSuffix));
        }
    }
    
    // Dumps the tree into a String.
    private String ps_;
    public String toString() {
        ps_ = "";
        StringBuffer sb = new StringBuffer("SuffixTree[ " + (nodes_ + 1)+ " nodes\n");
        for (int child = child_[root_]; child > 0; child = sibling_[child]) {
            toString(child, "(" + index_[root_] + ")<root>-", sb);
        }
        sb.append("]\n");
        ps_ = null;
        return sb.toString();
    }
    
    // Auxiliary method for dumping the tree into a String
    // by traversing the tree.
    private void toString(int node, String s, StringBuffer sb) {
        if (child_[node] > 0) {
            // inner node
            s += "(" + index_[node]+ ")" + 
                substring(index_[node], (index_[child_[node]] - index_[node]), false) + "-";
            for (int child = child_[node]; child > 0; child = sibling_[child]) {
                toString(child, s, sb);
            }
        }
        else {
            // leaf
            s = "  " + s + "(" + index_[node] + ")" + (-child_[node]) + "\n";
            if (ps_.length() > 0) {
                StringBuffer t = new StringBuffer(s.length());
                int i = 0;
                while((i < ps_.length()) && (i < s.length())) {
                    if (ps_.charAt(i) == s.charAt(i)) {
                        t.append(' ');
                    }
                    else {
                        break;
                    }
                    i ++;
                }
                String ss = t.toString();
                sb.append(ss.substring(0, ss.length() - 2) + s.substring(i - 2));
            }
            else {
                sb.append(s);
            }
            ps_ = s;
        }
    }
    
    // Increase node array capacity.
    private void increaseCapacity() {
        int size = nodes_ + increment_;
        System.err.println("- increasing node array capacity to: " + size);
        wasResized_ = true;
        int [] tmp = index_;
        index_ = new int[size];
        System.arraycopy(tmp, 0, index_, 0, tmp.length);
        tmp = child_;
        child_ = new int[size];
        System.arraycopy(tmp, 0, child_, 0, tmp.length);
        tmp = sibling_;
        sibling_ = new int[size];
        System.arraycopy(tmp, 0, sibling_, 0, tmp.length);        
    }
    
    // Set the node array capacity.
    private void setCapacity(int size) {
        System.err.println("- setting node array capacity to: " + size);
        int [] tmp = index_;
        index_ = new int[size];
        System.arraycopy(tmp, 0, index_, 0, size);
        tmp = child_;
        child_ = new int[size];
        System.arraycopy(tmp, 0, child_, 0, size);
        tmp = sibling_;
        sibling_ = new int[size];
        System.arraycopy(tmp, 0, sibling_, 0, size);        
        wasResized_ = false;
    }
    
    // Create a new node.
    private int newNode(int index, int child, int sibling) {
        nodes_ ++;
        if (nodes_ == index_.length) {
            increaseCapacity();
        }           
        index_[nodes_] = index;
        child_[nodes_] = child;
        sibling_[nodes_] = sibling;
        return nodes_;
    }
    
    // testing
    public static void main(String[] args) throws Exception {
        if (args.length > 4) {
            int capacity = Integer.parseInt(args[0]);
            int increment = Integer.parseInt(args[1]);
            Sequence[] s = new Sequence[args.length - 3];
            for (int i = 0; i < s.length; i ++) {
                s[i] = new Sequence(args[i + 3]);
                System.err.println(s[i]);
            }
            SuffixTree st = new SuffixTree(capacity, increment, s, true);
            //System.err.println(st);
            Result[] r = st.search(args[2]);
            if (r.length > 0) {
                System.err.println (args[2] + " was found in sequences: ");
                for (int i = 0; i < r.length; i ++) {
                    System.err.println(" - " + r[i].seqNo + ": " + 
                        st.sequence(r[i].seqNo) + " at index: " + r[i].suffix);
                }
            }
            else {
                System.err.println (args[2] + " was not found.");
            }
        }
        else {
            System.err.println("Call: java -cp classes SuffixTree <initial node capacity> " + 
                "<capacity increment> <searched string> <sequence 1> ... <sequence n>");
        }
    }
}
