package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class share {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-share_id", description = "Id of the share record")
    private int share_id;

    @Parameter(names = "log_id", description = "Id of the shared log")
    private int log_id;

    @Parameter(names = "-content", description = "Content of the share activity")
    private String content;

    @Parameter(names = "share_time", description = "Time when the log is shared")
    private String share_time;

    /**
     * @return the content
     */
    public String getContent() {
        return content;
    }

    /**
     * @return the log_id
     */
    public int getLog_id() {
        return log_id;
    }

    /**
     * @return the share_id
     */
    public int getShare_id() {
        return share_id;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }

    /**
     * @return the share_time
     */
    public String getShare_time() {
        return share_time;
    }
}