import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.*;

public class Main {
    public static void main(String[] args) throws Exception {
        String sql = ""; // the final SQL sentence to be executed
        // JDBC driver name
        final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
        // JDBC url
        final String DB_URL = "jdbc:mysql://localhost:3306/"; // in order to fix the
        // query contains Chinese
        // words
        final String USER = "root";
        final String PASS = "123456";
        Connection connection = null;
        Statement statement = null;
        try {
            // register JDBC driver
            Class.forName(JDBC_DRIVER);

            // establish connection with sql server
            System.out.println("Connecting to SQL...");
            connection = DriverManager.getConnection(DB_URL, USER, PASS);

            statement = connection.createStatement();
            // System.out.println("The SQL to be executed is: ");
            // System.out.println(sql);
            // ResultSet resultSet = statement.executeQuery(sql);
            // printTable(resultSet);
            // close result set to release memory
            // resultSet.close();
            createDataBase(statement);
            statement.close();
            connection.close();
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
                if (connection != null)
                    connection.close();
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

    static void createDataBase(Statement statement) throws IOException, SQLException {
        String fileName = "create_database.sql";
        BufferedReader bufferedReader = new BufferedReader(new FileReader(fileName));
        String line;
        String content = "";
        while ((line = bufferedReader.readLine()) != null) {
            if (line.length() > 0 && line.charAt(0) != '-') {
                content += line;
                content += "\n";
            }
        }
        String[] SQLs = content.split(";");
        for (String sql : SQLs) {
            if (!sql.equals("\n")) {
                System.out.println(sql);
                statement.execute(sql);
            }
        }
        bufferedReader.close();
        System.out.println("Create database successfully");
    }
}