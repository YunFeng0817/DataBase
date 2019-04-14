package command;

import com.beust.jcommander.*;
import java.sql.*;
import table.*;

public class show_emails extends command {

    @ParametersDelegate
    private email email = new email();

    @Override
    public void run(Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        String sql = "select user.user_id, user.name, email.address from user, email where user.user_id=email.user_id group by user.user_id,user.name,email.address";
        ResultSet resultSet = statement.executeQuery(sql);
        printTable(resultSet);
        resultSet.close();
    }

}