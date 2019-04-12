package table;

import java.util.ArrayList;
import java.util.List;

import com.beust.jcommander.*;

public class job {
    @Parameter
    private List<String> parameters = new ArrayList<String>();

    @Parameter(names = "-id", hidden = true)
    private int userID;

    @Parameter(names = "-job_id", description = "Id of the work experience record")
    private int job_id;

    @Parameter(names = "-employer", description = "Your employer details")
    private String employer;

    @Parameter(names = "-start", description = "The date when you start this period of job")
    private String start;

    @Parameter(names = "-end", description = "The date when you end this period of job")
    private String end;

    @Parameter(names = "-position", description = "Your job position")
    private String position;

    /**
     * @return the userID
     */
    public int getUserID() {
        return userID;
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
     * @return the employer
     */
    public String getEmployer() {
        return employer;
    }

    /**
     * @return the job_id
     */
    public int getJob_id() {
        return job_id;
    }

    /**
     * @return the position
     */
    public String getPosition() {
        return position;
    }

}