import java.sql.*;

public class Main {
    public static void main(String[] args) {
        // JDBC driver name
        final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
        // JDBC url
        final String DB_URL = "jdbc:mysql://localhost:3306/COMPANY";
        final String USER = "root";
        final String PASS = "123456";
        Connection connect = null;
        Statement statement = null;
        try {
            // register JDBC driver
            Class.forName(JDBC_DRIVER);

            // establish connection with sql server
            System.out.println("Connecting to SQL...");
            connect = DriverManager.getConnection(DB_URL, USER, PASS);

            statement = connect.createStatement();
            String sql;
            sql = "SELECT * FROM EMPLOYEE";
            ResultSet resultSet = statement.executeQuery(sql);
            printTable(resultSet);
            // close result set to release memory
            resultSet.close();
            statement.close();
            connect.close();
        } catch (SQLException se) {
            se.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (statement != null)
                    statement.close();
            } catch (SQLException se2) {
            }
            try {
                if (connect != null)
                    connect.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }
        }
        System.out.println("Goodbye!");
    }

    /**
     * automatically print the whole table according to query result
     * 
     * @param resultSet SQL query result
     * @throws SQLException parse result exception
     */
    static void printTable(ResultSet resultSet) throws SQLException {
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