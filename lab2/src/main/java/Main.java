import java.sql.*;

public class Main {
    public static void main(String[] args) {
        String sql = "";
        try {
            int choice;
            if (args.length >= 5) {
                if (args[0].equals("company_query") && args[1].equals("-q") && args[3].equals("-p")) {
                    choice = Integer.parseInt(args[2]);
                    switch (choice) {
                    case 1:
                        String parameters = "";
                        for (int i = 5; i <= args.length; i++) {
                            parameters += "'" + args[i - 1] + "'";
                            if (i != args.length)
                                parameters += ",";
                        }
                        sql = "select ESSN from WORKS_ON where PNO in (" + parameters + ");";
                        break;
                    case 2:
                        sql = "select ENAME from EMPLOYEE,PROJECT,WORKS_ON where EMPLOYEE.ESSN=WORKS_ON.ESSN and PROJECT.PNO=WORKS_ON.PNO and PNAME='"
                                + args[4] + "';";
                        break;
                    case 3:
                        sql = "select ENAME,ADDRESS from EMPLOYEE,DEPARTMENT where EMPLOYEE.DNO=DEPARTMENT.DNO and DNAME='"
                                + args[4] + "';";
                        break;
                    case 4:
                        int salaryLimit = Integer.parseInt(args[5]);
                        sql = "select ENAME,ADDRESS from EMPLOYEE,DEPARTMENT where EMPLOYEE.DNO=DEPARTMENT.DNO and DNAME='"
                                + args[4] + "' and SALARY<" + salaryLimit + ";";
                        break;
                    case 5:
                        sql = "select ENAME from EMPLOYEE where ESSN not in(select ESSN from WORKS_ON where PNO='"
                                + args[4] + "');";
                        break;
                    case 6:
                        sql = "select ENAME,DNAME from EMPLOYEE,DEPARTMENT where SUPERSSN=(select ESSN from EMPLOYEE where ENAME='"
                                + args[4] + "') and EMPLOYEE.DNO=DEPARTMENT.DNO;";
                        break;
                    case 7:
                        sql = "select ESSN from EMPLOYEE where ESSN in(select ESSN from WORKS_ON where PNO='" + args[4]
                                + "') and ESSN in(select ESSN from WORKS_ON where PNO='" + args[5] + "')";
                        break;
                    case 8:
                        int salary = Integer.parseInt(args[4]);
                        sql = "select DNAME from EMPLOYEE,DEPARTMENT where EMPLOYEE.DNO=DEPARTMENT.DNO group by DNAME having avg(SALARY)<"
                                + salary + ";";
                        break;
                    case 9:
                        int projectNumLimit = Integer.parseInt(args[4]);
                        int hourLimit = Integer.parseInt(args[5]);
                        sql = "select ENAME from EMPLOYEE where ESSN in (select ESSN from WORKS_ON group by ESSN having count(distinct PNO)>="
                                + projectNumLimit + " and sum(HOURS)<=" + hourLimit + ");";
                        break;
                    default:
                        throw new Exception(); // the <number> value not illegal
                    }
                }
            } else {
                throw new Exception(); // the format not illegal
            }
        } catch (Exception e) {
            System.err.println("Usage:\ncompany_query –q <Number> -p [Parameters]");
            return;
        }
        // JDBC driver name
        final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
        // JDBC url
        final String DB_URL = "jdbc:mysql://localhost:3306/COMPANY?characterEncoding=utf-8"; // in order to fix the
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
            System.out.println("The SQL to be executed is: ");
            System.out.println(sql);
            ResultSet resultSet = statement.executeQuery(sql);
            printTable(resultSet);
            // close result set to release memory
            resultSet.close();
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