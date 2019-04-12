package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class friend {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-friend_id", description = "Id of your friend")
    private int friend_id;

    @Parameter(names = "-group_id", description = "Id of your friend group")
    private int group_id;

    /**
     * @return the friend_id
     */
    public int getFriend_id() {
        return friend_id;
    }

    /**
     * @return the group_id
     */
    public int getGroup_id() {
        return group_id;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }
}