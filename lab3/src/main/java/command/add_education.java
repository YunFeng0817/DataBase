package command;

import com.beust.jcommander.*;
import java.sql.*;

import table.education;

@Parameters(commandDescription = "Add your education information")
public class add_education extends command {
    @ParametersDelegate
    private education education = new education();

    @Override
    public void run(Connection connection) throws SQLException {
        PreparedStatement statement = connection.prepareStatement(
                "insert into education(user_id,level,start,end,school_name,degree) values(?,?,?,?,?,?)",
                Statement.RETURN_GENERATED_KEYS);
        statement.setInt(1, education.getUserID());
        statement.setString(2, education.getLevel());
        statement.setString(3, education.getStart());
        statement.setString(4, education.getEnd());
        statement.setString(5, education.getSchoolName());
        statement.setString(6, education.getDegree());
        statement.executeUpdate();
        ResultSet keys = statement.getGeneratedKeys();
        int id;
        if (keys.next()) {
            id = keys.getInt(1);
            System.out.println("New education ID : " + id);
            printTable(connection.createStatement()
                    .executeQuery("select * from education where education_id = " + id + ";"));
        }
        keys.close();

    }

}