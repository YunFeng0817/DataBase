package command;

import com.beust.jcommander.*;
import java.sql.*;

import table.education;

@Parameters(commandDescription = "Add your education information")
public class delete_education extends command {
    @ParametersDelegate
    private education education = new education();

    @Override
    public void run(Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        statement.execute("delete from education where education_id='" + education.getEducation_id() + "';");
        statement.close();

    }

}