import time
from datetime import datetime

import psycopg2

from config import read_config
from messages import *
from user import User

POSTGRESQL_CONFIG_FILE_NAME = "database.cfg"

sign_in_timer = 0
sign_out_timer = 0

"""
    Connects to PostgreSQL database and returns connection object.
"""


def connect_to_db():
    db_conn_params = read_config(filename=POSTGRESQL_CONFIG_FILE_NAME, section="postgresql")
    conn = psycopg2.connect(**db_conn_params)
    conn.autocommit = False
    return conn


"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""


def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]


"""
    Prints list of available commands of the software.
"""


def help(conn,user):
    # TODO: Create behaviour of the application for different type of users: Non Authorized (not signed id), Free and Premium users. 
    print("\n*** Please enter one of the following commands ***")
    print("> help")
    print("> sign_up <user_id> <first_name> <last_name>")
    print("> sign_in <user_id>")
    if user:
        try:
            prem_query = "select * " \
                         "from \"Subscription\" " \
                         "where user_id = %s;"
            cursor = conn.cursor()
            cursor.execute(prem_query,(user.user_id,))
            prem = cursor.fetchone()
            print("> sign_out")
            print("> show_memberships")
            print("> show_subscription")
            print("> subscribe <membership_id>")
            print("> review <review_id> <business_id> <stars>")
            print("> search_for_businesses <keyword_1> <keyword_2> <keyword_3> ... <keyword_n>")
            if prem is not None:
                print("> suggest_businesses")
                print("> get_coupon")
        except (Exception, psycopg2.DatabaseError):
            pass
    print("> quit")



"""
    Saves user with given details.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_up(conn, user_id, user_name):
    # TODO: Implement this function
    sign_up_user = "insert into Users(user_id,user_name,review_count,yelping_since,useful,funny," \
                   "cool,fans,average_stars,session_count) values(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s);"

    sign_up_exist = "select user_id from Users where user_id = %s;"

    try:
        cursor = conn.cursor()
        cursor.execute(sign_up_exist, (user_id,))
        if cursor.fetchone() is not None:
            cursor.close()
            return False, CMD_EXECUTION_FAILED
        cursor.execute(sign_up_user,(user_id, user_name, 0, datetime.now(), 0, 0, 0, 0, 0, 0))
        conn.commit()
        cursor.close()
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Retrieves user information if there is a user with given user_id and user's session_count < max_parallel_sessions.
    - Return type is a tuple, 1st element is a user object and 2nd element is the response message from messages.py.
    - If there is no such user, return tuple (None, USER_SIGNIN_FAILED).
    - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (user, CMD_EXECUTION_SUCCESS).
    - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
"""


def sign_in(conn, user_id):
    # TODO: Implement this function
    sign_in_user_exist = "select * from Users where user_id = %s;"
    update_user = "update Users " \
                  "set session_count = session_count+1 " \
                  "where user_id=%s;"
    try:
        global sign_in_timer
        cursor = conn.cursor()
        cursor.execute(sign_in_user_exist,(user_id,))
        user_result = cursor.fetchone()
        if user_result is None:
            return None, USER_SIGNIN_FAILED
        else:
            user_subscription = "select Membership.max_parallel_sessions \
                                from Membership , \"Subscription\" \
                                where Membership.membership_id = \"Subscription\".membership_id " \
                                "and \"Subscription\".user_id = %s; "
            cursor.execute(user_subscription,(user_id,))
            user_plan = cursor.fetchone()
            if user_plan is None:
                if user_result[9] >= 1:
                    conn.rollback()
                    cursor.close()
                    return None, USER_ALL_SESSIONS_ARE_USED
                else:
                    sign_in_timer = time.time()
                    cursor.execute(update_user,(user_id,))
                    conn.commit()
                    cursor.close()
                    return User(user_result[0],user_result[1],user_result[2],user_result[3],user_result[4],
                                user_result[5],user_result[6],user_result[7],user_result[8],
                                user_result[9]),CMD_EXECUTION_SUCCESS
            else:
                if user_result[9] >= user_plan[0]:
                    conn.rollback()
                    cursor.close()
                    return None, USER_ALL_SESSIONS_ARE_USED
                else:
                    sign_in_timer = time.time()
                    cursor.execute(update_user, (user_id,))
                    conn.commit()
                    cursor.close()
                    return User(user_result[0], user_result[1], user_result[2], user_result[3], user_result[4],
                                user_result[5], user_result[6], user_result[7], user_result[8],
                                user_result[9]), CMD_EXECUTION_SUCCESS

    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return None, USER_SIGNIN_FAILED


"""
    Signs out from given user's account.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Decrement session_count of the user in the database.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def sign_out(conn, user):
    # TODO: Implement this function
    find_user = "select * from Users where user_id = %s;"
    update_user = "update Users " \
                  "set session_count = session_count-1 " \
                  "where user_id=%s;"
    update_subscription = "update \"Subscription\" " \
                          "set time_spent = time_spent + %s " \
                          "where user_id = %s;"
    try:
        global sign_out_timer
        cursor = conn.cursor()
        cursor.execute(find_user,(user.user_id,))
        sign_out_user = cursor.fetchone()
        if sign_out_user is None or sign_out_user[9] == 0:
            conn.rollback()
            cursor.close()
            return False, CMD_EXECUTION_FAILED
        else:
            sign_out_timer = time.time()
            spent = round(((sign_out_timer - sign_in_timer) * 1000), 2)
            cursor.execute(update_subscription,(spent,user.user_id))
            cursor.execute(update_user,(user.user_id,))
            conn.commit()
            cursor.close()
            return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Quits from program.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Remember to sign authenticated user out first.
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def quit(conn, user):
    # TODO: Implement this function
    try:
        if user is not None:
            return sign_out(conn,user)
        else:
            return True,CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        return False, CMD_EXECUTION_FAILED


"""
    Retrieves all available memberships and prints them.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print available memberships and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Max Sessions|Monthly Fee
    1|Silver|2|30
    2|Gold|4|50
    3|Platinum|10|90
"""


def show_memberships(conn,user):
    # TODO: Implement this function
    memberships_query = "select * from Membership;"
    try:
        cursor = conn.cursor()
        cursor.execute(memberships_query)
        memberships = cursor.fetchall()
        print("#|Name|Max Sessions|Monthly Fee")
        for i in range(len(memberships)):
            print(str(memberships[i][0])+"|"+str(memberships[i][1])+"|"+str(memberships[i][2])+"|"+str(memberships[i][3]))
        cursor.close
        return True,CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Retrieves authenticated user's membership and prints it. 
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If the operation is successful; print the authenticated user's membership and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    #|Name|Max Sessions|Monthly Fee
    2|Gold|4|50
"""


def show_subscription(conn, user):
    # TODO: Implement this function
    subscription_query = "select mb.membership_id, mb.membership_name, mb.max_parallel_sessions, mb.monthly_fee " \
                         "from Membership mb,\"Subscription\" sb " \
                         "where sb.membership_id = mb.membership_id and sb.user_id = %s;"
    try:
        cursor = conn.cursor()
        cursor.execute(subscription_query,(user.user_id,))
        subscription = cursor.fetchone()
        if subscription is None:
            return False, CMD_EXECUTION_FAILED
        print("#|Name|Max Sessions|Monthly Fee")
        print(str(subscription[0])+"|"+str(subscription[1])+"|"+str(subscription[2])+"|"+str(subscription[3]))
        cursor.close()
        return True,CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Insert user-review-business relationship to Review table if not exists in Review table.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - If a user-review-business relationship already exists (checking review_id is enough), do nothing on the database and return (True, CMD_EXECUTION_SUCCESS).
    - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If the business_id is incorrect; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
"""


def review(conn, user, review_id, business_id, stars):
    # TODO: Implement this function
    check_review_query = "select * " \
                         "from Review " \
                         "where review_id = %s;"
    check_business_query = "select * " \
                           "from Business " \
                           "where business_id = %s;"
    add_review_query = "insert into Review(review_id,user_id,business_id,stars,date,useful,funny,cool) " \
                       "values (%s,%s,%s,%s,%s,0,0,0);"
    try:
        cursor = conn.cursor()
        cursor.execute(check_review_query,(review_id,))
        check_review = cursor.fetchone()
        cursor.execute(check_business_query,(business_id,))
        check_business = cursor.fetchone()
        if check_review is not None or check_business is None:
            conn.rollback()
            cursor.close()
            return False, NOT_PERMITTED
        else:
            cursor.execute(add_review_query,(review_id,user.user_id,business_id,stars,datetime.now()))
            conn.commit()
            cursor.close
            return True,CMD_EXECUTION_SUCCESS

    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Subscribe authenticated user to new membership.
    - Return type is a tuple, 1st element is a user object and 2nd element is the response message from messages.py.
    - If target membership does not exist on the database, return tuple (None, SUBSCRIBE_MEMBERSHIP_NOT_FOUND).
    - If the new membership's max_parallel_sessions < current membership's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
    - If the operation is successful, commit changes and return tuple (user, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
"""


def subscribe(conn, user, membership_id):
    # TODO: Implement this function
    subscription_query = "select mb.membership_id, mb.membership_name, mb.max_parallel_sessions, mb.monthly_fee " \
                         "from Membership mb,\"Subscription\" sb " \
                         "where sb.membership_id = mb.membership_id and sb.user_id = %s;"
    membership_query = "select max_parallel_sessions " \
                       "from Membership " \
                       "where membership_id = %s;"
    update_subscription_query = "update \"Subscription\" " \
                                "set membership_id = %s, time_spent=0 " \
                                "where user_id = %s;"
    insert_subscription_query = "insert into \"Subscription\"(user_id,membership_id,time_spent) values (%s,%s,0);"
    try:
        cursor = conn.cursor()
        cursor.execute(subscription_query,(user.user_id,))
        subscription = cursor.fetchone()
        if subscription is not None:
            cursor.execute(membership_query,(membership_id,))
            membership = cursor.fetchone()
            if membership is None:
                conn.rollback()
                cursor.close()
                return None, SUBSCRIBE_MEMBERSHIP_NOT_FOUND
            if membership[0] < subscription[2]:
                return None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE
            else:
                cursor.execute(update_subscription_query,(membership_id,user.user_id))
                conn.commit()
                cursor.close()
                return user, CMD_EXECUTION_SUCCESS
        else:
            cursor.execute(insert_subscription_query,(user.user_id,membership_id))
            conn.commit()
            cursor.close()
            return user, CMD_EXECUTION_SUCCESS
        cursor.close()
        return True,CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return None, CMD_EXECUTION_FAILED

"""
    Searches for businesses with given search_text.
    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
    - Print all businesses whose names contain given search_text IN CASE-INSENSITIVE MANNER.
    - If the operation is successful; print businesses found and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

    Output should be like:
    Id|Name|State|Is_open|Stars
    1|A4 Coffee Ankara|ANK|1|4
    2|Tetra N Caffeine Coffee Ankara|ANK|1|4
    3|Grano Coffee Ankara|ANK|1|5
"""


def search_for_businesses(conn, user, search_text):
    # TODO: Implement this function
    search_query = "select business_id, business_name, state, is_open, stars " \
                   "from Business " \
                   "where business_name ilike (%s) escape '' order by business_id;"
    try:
        cursor = conn.cursor()
        cursor.execute(search_query,('%'+search_text+'%',))
        businesses = cursor.fetchall()
        print("Id|Name|State|Is_open|Starts")
        for b in businesses:
            print("%s|%s|%s|%r|%f" % (b[0], b[1], b[2], int(b[3]), b[4]))
        cursor.close()
        return True,CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED


"""
    Suggests combination of these businesses:

        1- Gather the reviews of that user.  From these reviews, find the top state by the reviewed business count.  
        Then, from all open businesses find the businesses that is located in the found state.  
        You should collect top 5 businesses by stars.

        2- Perform the same thing on the Tip table instead of Review table.

        3- Again check the review table to find the businesses get top stars from that user.  
        Among them get the latest reviewed one.  Now you need to find open top 3 businesses that is located in the same state 
        and has the most stars (if there is an equality order by name and get top 3).


    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.    
    - Output format and return format are same with search_for_businesses.
    - Order these businesses by their business_id, in ascending order at the end.
    - If the operation is successful; print businesses suggested and return tuple (True, CMD_EXECUTION_SUCCESS).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
"""


def suggest_businesses(conn, user):
    # TODO: Implement this function

    suggest_query = "select business_id, business_name,state,is_open,stars " \
                    "from ((select * " \
                    "from business b2 " \
                    "where b2.is_open = true " \
                    "and b2.state in ( " \
                    "select b.state " \
                    "from business b, review r " \
                    "where b.business_id = r.business_id and r.user_id = %s " \
                    "group by b.state " \
                    "order by count(*) desc limit 1) order by b2.stars desc limit 5) " \
                    "union( " \
                    "select * " \
                    "from business b3 " \
                    "where b3.is_open = true " \
                    "and b3.state in ( " \
                    "select b.state " \
                    "from business b, tip t " \
                    "where b.business_id = t.business_id and t.user_id = %s " \
                    "group by b.state " \
                    "order by count(*) desc limit 1) order by b3.stars desc limit 5) " \
                    "union( " \
                    "select * from business b4 " \
                    "where b4.is_open = true and b4.state in ( " \
                    "select b.state " \
                    "from  business b, review r " \
                    "where b.business_id = r.business_id and user_id = %s " \
                    "order by r.stars, date desc limit 1) " \
                    "order by b4.stars desc, business_name limit 3)) as suggests " \
                    "order by business_id asc;"
    try:
        cursor = conn.cursor()
        cursor.execute(suggest_query,(user.user_id,user.user_id,user.user_id,))
        suggests = cursor.fetchall()
        print("Id|Name|State|Is_open|Starts")
        for s in suggests:
            print("%s|%s|%s|%r|%f" % (s[0], s[1], s[2], int(s[3]), s[4]))
        cursor.close()
        return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED

"""
    Create coupons for given user. Coupons should be created by following these steps:

        1- Calculate the score by using the following formula:
            Score = timespent + 10 * reviewcount

        2- Calculate discount percentage using the following formula (threshold given in messages.py):
            actual_discount_perc = score/threshold * 100

        3- If found percentage in step 2 is lower than 25% print the following:
            You don’t have enough score for coupons.

        4- Else if found percentage in step 2 is between 25-50% print the following:
            Creating X% discount coupon.

        5- Else create 50% coupon and remove extra time from user's time_spent:
            Creating 50% discount coupon.

    - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.    
    - If the operation is successful (step 4 or 5); return tuple (True, CMD_EXECUTION_SUCCESS).
    - If the operation is not successful (step 3); return tuple (False, CMD_EXECUTION_FAILED).
    - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).


"""

def get_coupon(conn, user):
    # threshold is defined in messages.py, you can directly use it.
    time_spent_query = "select time_spent " \
                       "from \"Subscription\" " \
                       "where user_id = %s;"
    review_count_query = "select count(*) " \
                         "from Review " \
                         "where user_id = %s " \
                         "group by user_id;"
    try:
        cursor = conn.cursor()
        cursor.execute(time_spent_query,(user.user_id,))
        premium = cursor.fetchone()
        if premium is None:
            return False, NOT_ALLOWED
        else:
            cursor.execute(review_count_query,(user.user_id,))
            review_count = cursor.fetchone()
            score = premium[0]+10*review_count[0]
            actual_discount_perc = (score / threshold) * 100
            if actual_discount_perc < 25:
                cursor.close()
                return False, NOT_ENOUGH_SCORE
            elif actual_discount_perc < 50:
                print("Creating X% discount coupon.")
                cursor.close()
                return True,CMD_EXECUTION_SUCCESS
            else:
                print("Creating 50% discount coupon.")
                cursor.close()
                return True, CMD_EXECUTION_SUCCESS
    except (Exception, psycopg2.DatabaseError):
        conn.rollback()
        cursor.close()
        return False, CMD_EXECUTION_FAILED
