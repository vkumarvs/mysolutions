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
 
// The result of a search in a multi sequence.
public class Result 
{
    // sequence number in multi-sequence
    public int seqNo;
    // starting index (suffix) in the sequence
    public int suffix;
    
    public Result(int seqNo, int suffix) {
        this.seqNo = seqNo;
        this.suffix = suffix;
    }
    
    public String toString() {
        String s = "Result[seqNo: " + seqNo + ", suffix: " + suffix + "]";
        return s;
    }
}
