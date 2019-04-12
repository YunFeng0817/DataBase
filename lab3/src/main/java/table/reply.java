package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class reply {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-reply_id", description = "Id of the reply")
    private int reply_id;

    @Parameter(names = "log_id", description = "Id of the replied log")
    private int log_id;

    @Parameter(names = "-replied_id", description = "ID of reply which is replied")
    private int replied_id;

    @Parameter(names = "-content", description = "Content of the reply")
    private String content;

    @Parameter(names = "-reply_time", description = "Time when the the log is replied")
    private String reply_time;

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
     * @return the replied_id
     */
    public int getReplied_id() {
        return replied_id;
    }

    /**
     * @return the reply_id
     */
    public int getReply_id() {
        return reply_id;
    }

    /**
     * @return the reply_time
     */
    public String getReply_time() {
        return reply_time;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }

}