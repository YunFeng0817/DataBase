package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class user {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-name", description = "Your user name", required = true)
    private String name;

    @Parameter(names = "-gender", description = "Your gender MALE or FEMALE")
    private GENDER gender;

    @Parameter(names = "-password", description = "Login password", password = true)
    private String password;

    @Parameter(names = "-email", description = "Your E-main address")
    private List<String> emails = new ArrayList<>();

    @Parameter(names = "-birthday", description = "Your birthday ,format in 'yyyy-mm-dd'")
    private String birthday;

    @Parameter(names = "-address", description = "Your residential address")
    private String address;

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }

    /**
     * @param userID the userID to set
     */
    public void setUserID(int userID) {
        this.userID = userID;
    }

    /**
     * @return the address
     */
    public String getAddress() {
        return address;
    }

    /**
     * @return the birthday
     */
    public String getBirthday() {
        return birthday;
    }

    /**
     * @return the emails
     */
    public List<String> getEmails() {
        return emails;
    }

    /**
     * @return the gender
     */
    public String getGender() {
        return gender.toString();
    }

    /**
     * @return the name
     */
    public String getName() {
        return name;
    }

    /**
     * @return the password
     */
    public String getPassword() {
        return password;
    }

    public enum GENDER {
        MALE, FEMALE
    }

}