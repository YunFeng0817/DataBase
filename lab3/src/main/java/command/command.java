package command;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;

public abstract class command {
    public abstract void run(Connection connection) throws SQLException;

    /**
     * automatically print the whole table according to query result
     * 
     * @param resultSet SQL query result
     * @throws SQLException parse result exception
     */
    static void printTable(ResultSet resultSet) throws SQLException {
        // see if the query result is NULL, if so, print empty result to the console
        if (!resultSet.next()) {
            System.out.println("Empty result");
            return;
        }
        resultSet.previous(); // when check if result is NULL, the cursor had been move forward one unit

        ResultSetMetaData resultMetaData = resultSet.getMetaData();
        // print table property name
        for (int i = 1; i <= resultMetaData.getColumnCount(); i++) {
            System.out.print(resultMetaData.getColumnName(i) + "\t");
        }
        System.out.println("");
        // print table value
        while (resultSet.next()) {
            for (int i = 1; i <= resultMetaData.getColumnCount(); i++) {
                String columnValue = resultSet.getString(i);
                System.out.print(columnValue + "\t");
            }
            System.out.println("");
        }
    }
}