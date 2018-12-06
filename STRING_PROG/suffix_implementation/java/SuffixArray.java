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

// The Suffix Array
public class SuffixArray implements java.io.Serializable 
{   
    // Starting indices for the sequences in from the sequence array.
    protected int[] offset_;
    // the bytes of multi-sequence
    protected byte[] bytes_;
    // Min and max position in search
    private int seqCnt_;
    // End Of Sequence
    public static final byte EOS = (byte)'.';
    // Length (all the characters in sequences) of multi-sequence
    protected int length_;
    // Suffix reversed lexical order
    public int[] pos = null;
    
    // Constructs a suffix array
    public SuffixArray(Sequence[] s) {
        Watch w1 = new Watch();
        w1.start();
        seqCnt_ = s.length;
        offset_ = new int[seqCnt_ + 1];
        offset_[0] = 0;
        for (int i = 0; i < seqCnt_; i ++) {
            offset_[i + 1] = offset_[i] + s[i].characters.length() + 1;
        }
        bytes_ = new byte[offset_[seqCnt_]];
        System.err.println();
        System.err.println("Total number of bytes (including sequence delimiters): " + bytes_.length);
        System.err.println();
        length_ = bytes_.length - seqCnt_;
        pos = new int[length_];
        int m = 0;
        for (int i = 0; i < seqCnt_; i ++) {
            // copy and convert the sequence bytes
            byte[] b = s[i].characters.getBytes();
            /// uncomment this to free the original sequence array
            ///s[i] = null;
            int k = offset_[i];
            System.arraycopy(b, 0, bytes_, k, b.length);
            for (int j = 0; j < b.length; j ++) {
                pos[m ++] = k ++; // same as: pos[k - i] = k;
            }
            // put marker
            bytes_[k] = EOS;
        }
        /// uncomment this to free the original sequence array
        ///s = null;
        /// free memory
        ///System.gc();
        System.err.print("Sorting suffixes ");
        // compute pos[]
        Watch w2 = new Watch();
        w2.start();
        quickSort(0, (length_ - 1));
        System.err.println("in " + w2.msecs() + " msec.");
        System.err.println("SuffixArray built in " + w1.msecs() + " msec.");
    }

    // Return the suffix starting at index 'index' in the multi-sequence.
    public String stringSuffix(int index) {
        int k = index;
        while(bytes_[k] != EOS) {
            k ++;
        }
        byte[] b = new byte[k - index];
        System.arraycopy(bytes_, index, b, 0, b.length);
        return new String(b);    
    }
    
    // Return the sequence with number 'seqNo' in the multi-sequence.
    public String sequence(int seqNo) {
        return stringSuffix(offset_[seqNo]);
    }
    
    // Return a substring starting at index 'index' in the multi-sequence,
    // with length 'length', eventually trimmed to be a valid-suffix.
    public String substring(int first, int length, boolean trim) {
        byte[] b = new byte[length];
        for (int i = 0; i < b.length; i++) {
            if (bytes_[first] == EOS) {
                if (trim) {
                    byte[] tmp = b;
                    b = new byte[i];
                    System.arraycopy(tmp, 0, b, 0, i);
                }
                else {
                    System.err.println("substring error");
                    for (int j = i; j < length; j++) {
                        b[j] = EOS;
                    }
                }
                break;
            }
            else {
                b[i] = bytes_[first];
            }
            first ++;
        }
        return new String(b);    
    }
    
    // Dump the SuffixArray into a String.
    public String toString() {
        return toString(Integer.MAX_VALUE);
    }
    
    // 
    public String toString(int N) {
        StringBuffer sb = new StringBuffer("SuffixArray[\n");
        for (int i = 0; i < length_; i++) {
            String s = stringSuffix(pos[i]);
            if (N < s.length()) {
                s = s.substring(0, N) + "...";
            }
            sb.append("  pos[" + i + "]: " + format(3, "" + pos[i]) + ", bytes: " + s + "\n");
        }
        sb.append("\n]");
        return sb.toString();
    }

    private static String format(int pos, String value) {
        String s = ((value == null) ? "" : value);
        for (int i = s.length(); i < pos; i++) {
            s = " " + s;
        }
        return s;
    }

    // Finds a sequence that contains the byte at index in multi-sequence
    public int findSequenceNo(int index) {
        int m = 0;
        int n = seqCnt_;
        int p = (m + n) >> 1;
        while ((m < p) && (p < n)) {
            if (offset_[p] > index) {
                n = p;
                p = (m + n) >> 1;
            }
            else if (offset_[p] < index) {
                m = p;
                p = (m + n) >> 1;
            }
            else {
                break;
            }
        }
        return p;
    }
    
    // sorts pos[] such that suffixes are in REVERSED lexicographic order
    protected void quickSort(int low, int high) {
        if (low < high) {
            int pivot = partition(low, high);
            quickSort(low, (pivot - 1));
            quickSort((pivot + 1), high);
        }
    }
    
    private int pivot_;
    
    private int partition(int low, int high) {
        int left = low + 1;
        int right = high;
        pivot_ = pos[low];
        while (left <= right) {
            // move left while item < pivot
            while((left <= right) && (compareToPivot(pos[left]) <= 0)) {
                left++;
            }
            // move right while item > pivot
            while((left <= right) && (compareToPivot(pos[right]) >= 0)) {
                right--;
            }
            if (left < right) {
                int temp = pos[left];
                pos[left] = pos[right];
                pos[right] = temp;
            }
        }   
        // right is final position for the pivot
        pos[low] = pos[right];
        pos[right] = pivot_;
        return right;
    }
            
    private int compareToPivot(int index) {
        int i = index, j = pivot_;
        while ((bytes_[i] != EOS) && (bytes_[j] != EOS)) {
            int r = bytes_[j] - bytes_[i];
            if (r != 0) {
                return r;
            }
            i ++; 
            j ++;
        }
        if (bytes_[i] == EOS) {
            return ((bytes_[j] == EOS) ? 0 : 1);
        }
        return -1;
    } 
}
