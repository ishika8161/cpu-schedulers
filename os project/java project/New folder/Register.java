import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
@WebServlet("/register")
public class RegisterServlet extends HttpServlet {
    private static final String DB_URL = "jdbc:mysql://localhost:3306/info";
    private static final String DB_USER = "yuvika";
    private static final String DB_PASSWORD = "1234";
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        PrintWriter out = resp.getWriter();
        String name = req.getParameter("name");
        int age = Integer.parseInt(req.getParameter("age"));
        String email = req.getParameter("email");
        if (name == null || name.isEmpty()) {
            out.println("Name is required.");
            return;
        } else if (age <= 0) {
            out.println("Invalid age.");
            return;
        } else if (!isValidEmail(email)) {
            out.println("Invalid email format.");
            return;
        }
        try (Connection connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASSWORD)) {
            PreparedStatement statement = connection.prepareStatement("INSERT INTO users (name, age, email) VALUES (?, ?, ?)");
            statement.setString(1, name);
            statement.setInt(2, age);
            statement.setString(3, email);
            int rowsAffected = statement.executeUpdate();
            if (rowsAffected > 0) {
                out.println("User successfully registered: " + name + " (" + email + ")");
            } else {
                out.println("Error: User could not be registered.");
            }
        } catch (SQLException e) {
            out.println("Error: " + e.getMessage());
        }
    }
    private boolean isValidEmail(String email) {
        String regex = "^[\\w!#$%&'*+/=?^_`{|}~-]+(?:\\.[\\w!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,6}$";
        return Pattern.compile(regex).matcher(email).matches();
    }
}
