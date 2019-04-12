package command;

import com.beust.jcommander.*;
import java.sql.*;

import table.user;

@Parameters(commandDescription = "Register a new user")
public class add_user implements command {
    @ParametersDelegate
    private user user = new user();

    public void run(Statement statement) throws SQLException {
        String sql = "insert into user(";
        String items = "";
        String values = "";
        System.out.println(user.getPassword());
        if (user.getPassword() != null) {
            items += (items.equals("") ? "" : ",") + "password";
            values += (values.equals("") ? "" : ",") + "'" + user.getPassword() + "'";
        }
        if (user.getName() != null) {
            items += (items.equals("") ? "" : ",") + "name";
            values += (values.equals("") ? "" : ",") + "'" + user.getName() + "'";
        }
        if (user.getBirthday() != null) {
            items += (items.equals("") ? "" : ",") + "birthday";
            values += (values.equals("") ? "" : ",") + "'" + user.getBirthday() + "'";
        }
        if (user.getGender() != null) {
            items += (items.equals("") ? "" : ",") + "gender";
            values += (values.equals("") ? "" : ",") + "'" + user.getGender() + "'";
        }
        if (user.getAddress() != null) {
            items += (items.equals("") ? "" : ",") + "address";
            values += (values.equals("") ? "" : "," + "'") + user.getAddress() + "'";
        }
        System.out.println(sql + items + ") values(" + values + ");");
        sql = "select user_id from user where name='" + user.getName() + "';";
        // statement.execute(sql + values + ") values(" + items + ");");
    }

}