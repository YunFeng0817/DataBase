package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class email {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-email", description = "Your email address")
    private String address;

    /**
     * @return the address
     */
    public String getAddress() {
        return address;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }
}