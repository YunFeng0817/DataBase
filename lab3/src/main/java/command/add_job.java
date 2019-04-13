package command;

import com.beust.jcommander.*;
import java.sql.*;

import table.job;

@Parameters(commandDescription = "Add your job information")
public class add_job extends command {
    @ParametersDelegate
    private job job = new job();

    @Override
    public void run(Connection connection) throws SQLException {
        PreparedStatement statement = connection.prepareStatement(
                "insert into work_experience(user_id,employer,start,end,position) values(?,?,?,?,?)",
                Statement.RETURN_GENERATED_KEYS);
        statement.setInt(1, job.getUserID());
        statement.setString(2, job.getEmployer());
        statement.setString(3, job.getStart());
        statement.setString(4, job.getEnd());
        statement.setString(5, job.getPosition());
        statement.executeUpdate();
        ResultSet keys = statement.getGeneratedKeys();
        int id;
        if (keys.next()) {
            id = keys.getInt(1);
            System.out.println("New job ID : " + id);
            printTable(connection.createStatement().executeQuery("select * from work_experience where experience_id = " + id + ";"));
        }
        keys.close();

    }

}