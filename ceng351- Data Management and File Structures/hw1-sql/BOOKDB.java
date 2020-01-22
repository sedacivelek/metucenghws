package ceng.ceng351.bookdb;

import ceng.ceng351.bookdb.QueryResult.ResultQ1;
import ceng.ceng351.bookdb.QueryResult.ResultQ2;
import ceng.ceng351.bookdb.QueryResult.ResultQ3;
import ceng.ceng351.bookdb.QueryResult.ResultQ4;
import ceng.ceng351.bookdb.QueryResult.ResultQ5;
import ceng.ceng351.bookdb.QueryResult.ResultQ6;
import ceng.ceng351.bookdb.QueryResult.ResultQ7;
import ceng.ceng351.bookdb.QueryResult.ResultQ8;
import com.mysql.cj.protocol.Resultset;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Arrays;
/**
 *
 * @author Seda Civelek
 */
public class BOOKDB implements IBOOKDB{
    private static String user = "";//"pa1_user";
    private static String password = ""; //"123456";
    private static String host = "144.122.71.65"; //"localhost";
    private static String database = "";//"pa1_recitation";
    private static int port = 8084;

    private Connection con=null;
    public BOOKDB(){
    }

    @Override
    public void initialize() {
        String url = "jdbc:mysql://"+this.host + ":" +this.port +"/"+this.database;
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            con = DriverManager.getConnection(url,this.user,this.password);
        }
        catch(SQLException | ClassNotFoundException e){
            e.printStackTrace();
        }
    }

    @Override
    public int createTables(){
        int result;
        int numberOfTablesCreated =0;
        String queryCreateAuthorTable = "CREATE TABLE author("+
                "author_id int,"+
                "author_name varchar(60),"+
                "PRIMARY KEY(author_id));";
        String queryCreatePublisherTable = "CREATE TABLE publisher("+
                "publisher_id int,"+
                "publisher_name varchar(50),"+
                "PRIMARY KEY(publisher_id));";
        String queryCreateBookTable = "CREATE TABLE book("+
                "isbn char(13),"+
                "book_name varchar(120),"+
                "publisher_id int,"+
                "first_publish_year char(4),"+
                "page_count int,"+
                "category varchar(25),"+
                "rating float,"+
                "PRIMARY KEY(isbn),"+
                "FOREIGN KEY (publisher_id) REFERENCES publisher(publisher_id) ON DELETE CASCADE ON UPDATE CASCADE);";
        String queryCreateAuthorOfTable = "CREATE TABLE author_of("+
                "isbn char(13),"+
                "author_id int,"+
                "PRIMARY KEY(isbn,author_id),"+
                "FOREIGN KEY (isbn) REFERENCES book(isbn) ON DELETE CASCADE ON UPDATE CASCADE,"+
                "FOREIGN KEY (author_id) REFERENCES author(author_id) ON DELETE CASCADE ON UPDATE CASCADE);";
        String queryCreatePhw1Table = "CREATE TABLE phw1("+
                "isbn char(13),"+
                "book_name varchar(120),"+
                "rating float,"+
                "PRIMARY KEY(isbn));";
        try {
            Statement statement = this.con.createStatement();
            result = statement.executeUpdate(queryCreateAuthorTable);
            numberOfTablesCreated++;
            result = statement.executeUpdate(queryCreatePublisherTable);
            numberOfTablesCreated++;
            result = statement.executeUpdate(queryCreateBookTable);
            numberOfTablesCreated++;
            result = statement.executeUpdate(queryCreateAuthorOfTable);
            numberOfTablesCreated++;
            result = statement.executeUpdate(queryCreatePhw1Table);
            numberOfTablesCreated++;

            statement.close();
        }
        catch (SQLException e){
            e.printStackTrace();
        }
        return numberOfTablesCreated;
    }
    @Override
    public int dropTables(){
        int result;
        int numberOfTablesDropped=0;
        String dropAuthor = "drop table author;";
        String dropPublisher = "drop table publisher;";
        String dropBook = "drop table book;";
        String dropAuthorof = "drop table author_of;";
        String dropphw1 = "drop table phw1;";
        try{
            Statement statement = this.con.createStatement();
            result = statement.executeUpdate(dropAuthorof);
            numberOfTablesDropped++;
            result = statement.executeUpdate(dropBook);
            numberOfTablesDropped++;
            result = statement.executeUpdate(dropAuthor);
            numberOfTablesDropped++;
            result = statement.executeUpdate(dropPublisher);
            numberOfTablesDropped++;
            result = statement.executeUpdate(dropphw1);
            numberOfTablesDropped++;

            statement.close();
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return numberOfTablesDropped;
    }
    @Override
    public int insertAuthor(Author[] authors){
        int inserted=0;
        int result;
        int i;
        int size = authors.length;
        for(i=0;i<size;i++){
            String query = "insert into author values(\""+
                    authors[i].getAuthor_id()+"\",\""+
                    authors[i].getAuthor_name()+"\");";
            try{
                Statement statement = this.con.createStatement();
                result = statement.executeUpdate(query);
                inserted++;
                statement.close();
            }
            catch(SQLException e){
                e.printStackTrace();
            }
        }
        return inserted;
    }
    public int insertBook(Book[] books){
        int inserted=0;
        int result;
        int i;
        int size = books.length;
        for(i=0;i<size;i++){
            String query = "insert into book values (\""+
                    books[i].getIsbn()+"\",\""+
                    books[i].getBook_name()+"\",\""+
                    books[i].getPublisher_id()+"\",\""+
                    books[i].getFirst_publish_year()+"\",\""+
                    books[i].getPage_count()+"\",\""+
                    books[i].getCategory()+"\",\""+
                    books[i].getRating()+"\");";
            try{
                Statement statement = this.con.createStatement();
                result = statement.executeUpdate(query);
                inserted++;
                statement.close();
            }
            catch(SQLException e){
                e.printStackTrace();
            }
        }
        return inserted;
    }

    @Override
    public int insertPublisher(Publisher[] publishers){
        int inserted=0;
        int result;
        int i;
        int size=publishers.length;
        for(i=0;i<size;i++){
            String query = "insert into publisher values (\""+
                    publishers[i].getPublisher_id()+"\",\""+
                    publishers[i].getPublisher_name()+"\");";
            try{
                Statement statement = this.con.createStatement();
                result = statement.executeUpdate(query);
                inserted++;
                statement.close();
            }
            catch(SQLException e){
                e.printStackTrace();
            }
        }
        return inserted;
    }

    @Override
    public int  insertAuthor_of(Author_of[] author_ofs){
        int inserted=0;
        int result;
        int i;
        int size=author_ofs.length;
        for(i=0;i<size;i++){
            String query = "insert into author_of values (\""+
                    author_ofs[i].getIsbn()+"\",\""+
                    author_ofs[i].getAuthor_id()+"\");";
            try{
                Statement statement = this.con.createStatement();
                result = statement.executeUpdate(query);
                inserted++;
                statement.close();
            }
            catch(SQLException e){
                e.printStackTrace();
            }
        }
        return inserted;
    }

    @Override
    public ResultQ1[] functionQ1(){
        ResultQ1[] temp = new ResultQ1[0];
        String query = "select B.isbn, B.first_publish_year, B.page_count,P.publisher_name "+
                "from book B, publisher P "+
                "where B.publisher_id=P.publisher_id and B.page_count in(select max(B1.page_count) "+
                "from book B1) "+
                "order by B.isbn;";
        int i=0;
        try{
            Statement statement=this.con.createStatement();
            ResultSet rs=statement.executeQuery(query);
            while(rs.next()){
                String isbn=rs.getString("B.isbn");
                String fpy=rs.getString("B.first_publish_year");
                int pc=rs.getInt("B.page_count");
                String pn=rs.getString("P.publisher_name");
                ResultQ1 result = new ResultQ1(isbn,fpy,pc,pn);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2){
        ResultQ2[] temp = new ResultQ2[0];
        String query = "select B.publisher_id, avg(B.page_count) "+
                "from book B where B.publisher_id in "+
                "(select B1.publisher_id from author_of A1, book B1 "+
                "where A1.author_id="+author_id1+ " and A1.isbn=B1.isbn "+
                "and B1.publisher_id in "+
                "(select B2.publisher_id from author_of A2, book B2 "+
                "where A2.author_id="+author_id2+ " and A2.isbn=B2.isbn)) "+
                "group by B.publisher_id "
                + "order by B.publisher_id; ";
        int i=0;
        try{
            Statement statement=this.con.createStatement();
            ResultSet rs=statement.executeQuery(query);
            while(rs.next()){
                int pid=rs.getInt("B.publisher_id");
                double c=rs.getDouble("avg(B.page_count)");
                ResultQ2 result = new ResultQ2(pid,c);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public ResultQ3[] functionQ3(String author_name){
        ResultQ3[] temp = new ResultQ3[0];
        String query = "Select B.book_name, B.category, B.first_publish_year\n" +
                "from book B\n" +
                "where B.first_publish_year in (select min(B1.first_publish_year)\n" +
                "                from book B1, author_of AU, author A\n" +
                "                where A.author_id=AU.author_id and B1.isbn=AU.isbn and A.author_name=\"" +author_name +
                "\"     group by AU.author_id)\n" +
                "    order by B.book_name, B.category, B.first_publish_year;";
        int i=0;
        try{
            Statement statement=this.con.createStatement();
            ResultSet rs=statement.executeQuery(query);
            while(rs.next()){
                String name=rs.getString("B.book_name");
                String c=rs.getString("B.category");
                String fpy=rs.getString("B.first_publish_year");
                ResultQ3 result = new ResultQ3(name,c,fpy);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
        }


    @Override
    public ResultQ4[] functionQ4(){
        ResultQ4[] temp = new ResultQ4[0];
        String query = "select distinct B.publisher_id, B.category\n" +
                "from book B, publisher P\n" +
                "where B.publisher_id=P.publisher_id and P.publisher_name like \"_% %_ %_\"\n" +
                "and B.publisher_id in (select B1.publisher_id\n" +
                "    from book B1\n" +
                "    group by B1.publisher_id\n" +
                "    having 2<(select count(*)\n" +
                "            from book B2\n" +
                "            where B2.publisher_id=B1.publisher_id) and\n" +
                "            3<avg(B1.rating))\n" +
                "order by B.publisher_id, B.category;";
        int i=0;
        try{
            Statement statement = this.con.createStatement();
            ResultSet rs = statement.executeQuery(query);
            while(rs.next()){
                int id=rs.getInt("B.publisher_id");
                String name=rs.getString("B.category");
                ResultQ4 result = new ResultQ4(id,name);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public ResultQ5[] functionQ5(int author_id){
        ResultQ5[] temp = new ResultQ5[0];
        String query = "Select A.author_id, A.author_name\n" +
                "From author A\n" +
                "where not exists(select B.publisher_id\n" +
                "                from book B, author_of AU\n" +
                "                where B.isbn=AU.isbn  and AU.author_id="+author_id+"\n" +
                "                and B.publisher_id not in (select B1.publisher_id\n" +
                "                    from book B1,author_of AU1\n" +
                "                    where B1.isbn=AU1.isbn and AU1.author_id=A.author_id))\n" +
                "order by A.author_id;";
        int i=0;
        try{
            Statement statement = this.con.createStatement();
            ResultSet rs = statement.executeQuery(query);
            while(rs.next()){
                int id=rs.getInt("A.author_id");
                String name=rs.getString("A.author_name");
                ResultQ5 result = new ResultQ5(id,name);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public ResultQ6[] functionQ6(){
        ResultQ6[] temp = new ResultQ6[0];
        String query = "select A.author_id, A.isbn\n" +
                "from author_of A\n" +
                "where A.author_id in(select AU.author_id\n" +
                "                     from author_of AU\n" +
                "                     where not exists(\n" +
                "                             select B.isbn\n" +
                "                             from book B\n" +
                "                             where B.publisher_id in(select P.publisher_id\n" +
                "                                                     from book B1, publisher P\n" +
                "                                                     where B1.isbn=AU.isbn and P.publisher_id=B1.publisher_id) and\n" +
                "                                     B.isbn not in (select AU1.isbn\n" +
                "                                                    from author_of AU1\n" +
                "                                                    where AU1.author_id=AU.author_id)\n" +
                "                         ) )\n" +
                " order by A.author_id,A.isbn;";
        int i=0;
        try{
            Statement statement = this.con.createStatement();
            ResultSet rs = statement.executeQuery(query);
            while(rs.next()){
                int id=rs.getInt("A.author_id");
                String name=rs.getString("A.isbn");
                ResultQ6 result = new ResultQ6(id,name);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public ResultQ7[] functionQ7(double rating){
        ResultQ7[] temp = new ResultQ7[0];
        String query = "select P.publisher_id,P.publisher_name\n" +
                "from publisher P\n" +
                "where P.publisher_id in (select B.publisher_id\n" +
                "                         from book B\n" +
                "                         where B.category= \"Roman\"\n" +
                "                         group by B.publisher_id\n" +
                "                         having count(*)>1)\n" +
                "\n" +
                "  and P.publisher_id in (select B2.publisher_id\n" +
                "                         from book B2\n" +
                "                         group by B2.publisher_id\n" +
                "                         having "+rating +"< avg(B2.rating))\n" +
                "\n" +
                "order by P.publisher_id;";
        int i=0;
        try{
            Statement statement = this.con.createStatement();
            ResultSet rs = statement.executeQuery(query);
            while(rs.next()){
                int id=rs.getInt("P.publisher_id");
                String name=rs.getString("P.publisher_name");
                ResultQ7 result = new ResultQ7(id,name);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public ResultQ8[] functionQ8(){
        ResultQ8[] temp = new ResultQ8[0];
        String query = "Insert into phw1(isbn, book_name, rating)\n" +
                "select B.isbn, B.book_name,B.rating\n" +
                "from book B, book B1\n" +
                "where B.book_name=B1.book_name and B.isbn<>B1.isbn and B.rating=(select min(B2.rating)\n" +
                "                                                    from book B2\n" +
                "                                                    where B2.book_name=B.book_name) order by B.isbn;";
        String s = "select *\n" +
                "from phw1 P \n" +
                "order by P.isbn;";
        int i=0;
        try{
            Statement statement = this.con.createStatement();
            statement.executeUpdate(query);
            ResultSet rs = statement.executeQuery(s);
            while(rs.next()){
                String isb =rs.getString("P.isbn");
                String name=rs.getString("P.book_name");
                double rat = rs.getDouble("P.rating");
                ResultQ8 result = new ResultQ8(isb,name,rat);
                temp=Arrays.copyOf(temp,temp.length+1);
                temp[i]=result;
                i++;
            }
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return temp;
    }

    @Override
    public double functionQ9(String keyword){
        double sum = 0.0;
        String query = "update book\n" +
                "set rating=rating+1\n" +
                "where rating<=4 and book_name like \"%"+keyword+"%\";";
        String s = "select sum(rating)\n" +
                "from book;";
        try{
            Statement statement = this.con.createStatement();
            statement.executeUpdate(query);
            ResultSet rs = statement.executeQuery(s);
            rs.next();
            sum=rs.getDouble(1);
        }
        catch (SQLException e){
            e.printStackTrace();
        }
        return sum;
    }

    @Override
    public int function10(){
        int count=0;
        String query = "delete\n" +
                "from publisher\n" +
                "where publisher_id not in (select P.publisher_id\n" +
                "    from book P );";
        String s = "select count(*)\n" +
                "from publisher;";
        try{
            Statement statement = this.con.createStatement();
            statement.executeUpdate(query);
            ResultSet rs = statement.executeQuery(s);
            rs.next();
            count=rs.getInt(1);
        }
        catch (SQLException e){
            e.printStackTrace();
        }
        return count;
    }



}
