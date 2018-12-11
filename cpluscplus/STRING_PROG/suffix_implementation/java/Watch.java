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

// A simple watch to measure the time.
public class Watch {

    private long time_ = 0;
    private boolean inPause_ = false;

    // Start the watch
    public synchronized void start() {
        if (inPause_) {
            time_ = new Date().getTime() - time_;
            inPause_ = false;
        }
        else {
            time_ = new Date().getTime();
        }
    }

    // Pause the time measuring.
    public synchronized void pause() {
        time_ = new Date().getTime() - time_;
        inPause_ = true;
    }

    // Number of miliseconds since the watch was started, excluding pauses.
    public synchronized long msecs() {
        return (inPause_ ? time_ : (new Date().getTime() - time_));
    }

    // Number of seconds since the watch was started, excluding pauses.
    public long secs() {
        return (msecs() / 1000L);
    }

    // Number of minutes since the watch was started, excluding pauses.
    public long mins() {
        return (secs() / 60L);
    }

    // Number of hours since the watch was started, excluding pauses.
    public long hours() {
        return (mins() / 60L);
    }
}
