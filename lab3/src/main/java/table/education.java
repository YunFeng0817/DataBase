package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class education {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-education_id", description = "Id of the education record")
    int education_id;

    @Parameter(names = "-level", description = "Your education level")
    private String level;

    @Parameter(names = "-start", description = "The date when you start this period of education")
    private String start;

    @Parameter(names = "-end", description = "The date when you end this period of education")
    private String end;

    @Parameter(names = "-school_name", description = "Your school name")
    private String schoolName;

    @Parameter(names = "-degree", description = "Your degree level")
    private String degree;

    /**
     * @return the education_id
     */
    public int getEducation_id() {
        return education_id;
    }

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
    }

    /**
     * @return the degree
     */
    public String getDegree() {
        return degree;
    }

    /**
     * @return the level
     */
    public String getLevel() {
        return level;
    }

    /**
     * @return the start
     */
    public String getStart() {
        return start;
    }

    /**
     * @return the end
     */
    public String getEnd() {
        return end;
    }

    /**
     * @return the schoolName
     */
    public String getSchoolName() {
        return schoolName;
    }

}