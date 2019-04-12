package command;

import java.sql.SQLException;
import java.sql.Statement;

public interface command {
    public void run(Statement Statement) throws SQLException;
}