package command;

import java.sql.Connection;
import java.sql.SQLException;

import com.beust.jcommander.*;
import java.sql.*;
import table.user;

@Parameters(commandDescription = "Update your information")
public class update_user extends command {

    @ParametersDelegate
    private user user = new user();

    @Override
    public void run(Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        String sql = "update user set ";
        String items = "";
        if (user.getPassword() != null) {
            items += (items.equals("") ? "" : ",") + "password=" + "'" + user.getPassword() + "'";
        }
        if (user.getName() != null) {
            items += (items.equals("") ? "" : ",") + "name=" + "'" + user.getName() + "'";
        }
        if (user.getBirthday() != null) {
            items += (items.equals("") ? "" : ",") + "birthday=" + "'" + user.getBirthday() + "'";
        }
        if (user.getGender() != null) {
            items += (items.equals("") ? "" : ",") + "gender=" + "'" + user.getGender() + "'";
        }
        if (user.getAddress() != null) {
            items += (items.equals("") ? "" : ",") + "address=" + "'" + user.getAddress() + "'";
        }
        if (!items.equals(""))
            sql = sql + items + "where user_id=" + user.getUserID() + ";";
        statement.execute(sql);
        statement.close();
    }

}