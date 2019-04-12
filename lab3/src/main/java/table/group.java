package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class group {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-group_id", description = "The id of your friend group")
    private int group_id;

    @Parameter(names = "-name", description = "The name of your friend group")
    private String name;

    /**
     * @return the group_id
     */
    public int getGroup_id() {
        return group_id;
    }

    /**
     * @return the name
     */
    public String getName() {
        return name;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }
}