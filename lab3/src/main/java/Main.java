import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.*;
import java.lang.reflect.Method;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import com.beust.jcommander.*;

import command.*;

// add_user -name Tony -password 123456 -gender MALE -email 294865@qq.com -email 2888@qq.com

// add_education -level undergraduate -start 2016-09-01 -end 2020-06-01 -school_name HIT -degree master
// delete_education -education_id 4
public class Main {
    public static void main(String[] args) throws Exception {
        // JDBC driver name
        final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
        // JDBC url
        final String DB_URL = "jdbc:mysql://localhost:3306/social_network?characterEncoding=utf-8";
        final String USER = "root";
        final String PASS = "123456";
        Connection connection = null;

        String command;
        Scanner scanner = new Scanner(System.in);
        List<command> commandTypes;
        JCommander jCommander;

        int user_id = -1;
        String userName;

        try {
            // register JDBC driver
            Class.forName(JDBC_DRIVER);

            // establish connection with sql server
            System.out.println("Connecting to SQL...");
            connection = DriverManager.getConnection(DB_URL, USER, PASS);

            // createDataBase(statement);
            // used to login or register
            while (true) {
                System.out.println("1.login");
                System.out.println("2.register");
                System.out.println("3.exit");
                int choice = scanner.nextInt();
                if (choice == 1) { // user choose to login
                    System.out.println("Input your user name:");
                    scanner.nextLine(); // read the extra blank line
                    userName = scanner.nextLine();

                    System.out.println("Input your user password:");
                    String password = scanner.nextLine();
                    PreparedStatement statement = connection
                            .prepareStatement("select user_id, password from user where name=?;");
                    statement.setString(1, userName);
                    ResultSet resultSet = statement.executeQuery();
                    if (resultSet.next()) {
                        String realPassword = resultSet.getString("password");
                        user_id = resultSet.getInt("user_id");
                        if (realPassword.equals(password)) {
                            System.out.println("Login successfully!");
                            break;
                        } else {
                            System.err.println("Wrong password!");
                        }
                    } else {
                        userName = null;
                        System.err.println("The account doesn't exist!");
                    }
                } else if (choice == 2) { // user choose to sign up
                    System.out.println("Input your message: name and password required ");
                    System.out.print(">");
                    scanner.nextLine(); // read the extra blank line
                    command = scanner.nextLine();

                    add_user addUser = new add_user();
                    commandTypes = new ArrayList();
                    commandTypes.add((add_user) addUser);
                    jCommander = JCommander.newBuilder().addCommand("add_user", addUser).build();
                    try {
                        jCommander.parse(command.split(" "));
                        Class commandType = Class.forName("command." + jCommander.getParsedCommand());
                        Method declaredMethod = commandType.getDeclaredMethod("run", Connection.class);
                        for (command c : commandTypes) {
                            if (commandType.isInstance(c)) {
                                c.run(connection);
                            }
                        }
                    } catch (ParameterException e) {
                        jCommander.usage();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } else {
                    System.exit(0);
                }
            }
            // for user to operate the system
            while (true) {
                System.out.print(userName + ">");
                command = scanner.nextLine();
                if (!command.equals("exit")) {
                    add_user addUser = new add_user();
                    update_user update_user = new update_user();
                    add_education add_education = new add_education();
                    update_education update_education = new update_education();
                    delete_education delete_education = new delete_education();
                    commandTypes = new ArrayList();
                    commandTypes.add(addUser);
                    commandTypes.add(update_user);
                    commandTypes.add(add_education);
                    commandTypes.add(update_education);
                    commandTypes.add(delete_education);
                    jCommander = JCommander.newBuilder().addCommand("add_user", addUser)
                            .addCommand("add_education", add_education).addCommand("update_user", update_user)
                            .addCommand("delete_education", delete_education).build();
                    try {
                        command += " -id " + user_id;
                        jCommander.parse(command.split("\\s"));
                        Class commandType = Class.forName("command." + jCommander.getParsedCommand());
                        Method declaredMethod = commandType.getDeclaredMethod("run", Connection.class);
                        for (command c : commandTypes) {
                            if (commandType.isInstance(c)) {
                                c.run(connection);
                            }
                        }
                    } catch (ParameterException e) {
                        jCommander.usage();
                        e.printStackTrace();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } else
                    break;
            }
            scanner.close();
            System.out.println("Exit successfully!");
            // close result set to release memory
            connection.close();
        } catch (SQLException se) {
            se.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (connection != null)
                    connection.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }
        }
        System.out.println("Goodbye!");
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
                statement.execute(sql);
            }
        }
        bufferedReader.close();
        System.out.println("Create database successfully");
    }
}