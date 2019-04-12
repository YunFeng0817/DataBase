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
            values += (values.equals("") ? "" : ",") + "'" + user.getAddress() + "'";
        }
        sql = sql + items + ") values(" + values + ");";
        statement.execute(sql);
        sql = "select user_id from user where name='" + user.getName() + "';";
        ResultSet resultSet = statement.executeQuery(sql);
        resultSet.next();
        int user_id = resultSet.getInt("user_id");
        if (user.getEmails().size() > 0) {
            for (String email : user.getEmails()) {
                sql = "insert into email(user_id,address) values(" + user_id + ",'" + email + "');";
                statement.execute(sql);
            }
        }
    }

}