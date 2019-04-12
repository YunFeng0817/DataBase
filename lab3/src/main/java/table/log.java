package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class log {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-log_id", description = "ID of the log")
    private int log_id;

    @Parameter(names = "-content", description = "Content of the log")
    private String content;

    @Parameter(names = "-private", description = "Set the log to be private use true,otherwise use false")
    private boolean privateLog;

    @Parameter(names = "-update_time", description = "Last update time, format: yyyy-mm-dd")
    private String update_time;

    /**
     * @return the log_id
     */
    public int getLog_id() {
        return log_id;
    }

    /**
     * @return the update_time
     */
    public String getUpdate_time() {
        return update_time;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }

    /**
     * @return the privateLog
     */
    public boolean isPrivateLog() {
        return privateLog;
    }

    /**
     * @return the content
     */
    public String getContent() {
        return content;
    }
}