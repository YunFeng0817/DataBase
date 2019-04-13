package command;

import com.beust.jcommander.*;
import java.sql.*;

import table.education;

public class update_education extends command {

    @ParametersDelegate
    private education education = new education();

    @Override
    public void run(Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        String sql = "update education set ";
        String items = "";
        if (education.getDegree() != null) {
            items += (items.equals("") ? "" : ",") + "degree=" + "'" + education.getDegree() + "'";
        }
        if (education.getStart() != null) {
            items += (items.equals("") ? "" : ",") + "start=" + "'" + education.getStart() + "'";
        }
        if (education.getEnd() != null) {
            items += (items.equals("") ? "" : ",") + "end=" + "'" + education.getEnd() + "'";
        }
        if (education.getLevel() != null) {
            items += (items.equals("") ? "" : ",") + "level=" + "'" + education.getLevel() + "'";
        }
        if (education.getSchoolName() != null) {
            items += (items.equals("") ? "" : ",") + "school_name=" + "'" + education.getSchoolName() + "'";
        }
        if (!items.equals(""))
            sql = sql + items + "where user_id=" + user.getUserID() + ";";
        statement.execute(sql);
        statement.close();
    }

}