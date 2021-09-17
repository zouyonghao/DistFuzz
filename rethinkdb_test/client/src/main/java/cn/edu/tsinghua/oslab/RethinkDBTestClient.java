package cn.edu.tsinghua.oslab;

import java.util.HashMap;

import com.rethinkdb.RethinkDB;
import com.rethinkdb.net.Connection;

public class RethinkDBTestClient {
    private static final int[] PORTS = { 4000, 4001, 4002 };

    private static final String QUERY_KEY = "the_key";
    private static final String FIELD_KEY = "the_field_key";

    public RethinkDB r = RethinkDB.r;
    public Connection conn = null;

    public RethinkDBTestClient() {
        for (int port : PORTS) {
            try {
                conn = r.connection().hostname("127.0.1.1").port(port).timeout(3).connect();
                System.out.println("Connect success!");
                break;
            } catch (Exception e) {
                // ignored
            }
        }
        if (conn == null) {
            System.out.println("Can not connect to any rethinkdb server!");
            System.exit(1);
        }
    }

    public void init() {
        try {
            r.dbCreate("test").run(conn);
        } catch (Exception e) {
            // ignored for table exists
            // e.printStackTrace();
        }
        try {
            r.db("test").tableCreate("test").optArg("replicas", 3).run(conn);
        } catch (Exception e) {
            // e.printStackTrace();
        }
    }

    public int get() {
        try {
            Long value = r.db("test").table("test").get(QUERY_KEY).g(FIELD_KEY).run(conn);
            System.out.println(value);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public int set(long value) {
        try {
            r.db("test").table("test").insert(r.hashMap("id", QUERY_KEY).with(FIELD_KEY, value))
                    .optArg("conflict", "update").run(conn);
            System.out.println(value);
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public int cas(long oldValue, long newValue) {
        try {
            HashMap<String, Long> result = r.db("test").table("test")
                    .filter(row -> row.g("id").eq(QUERY_KEY).and(row.g(FIELD_KEY).eq(oldValue)))
                    .update(r.hashMap(FIELD_KEY, newValue)).run(conn);
            if (result.get("replaced") > 0) {
                System.out.println(newValue);
            } else {
                return 2;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return 1;
        }
        return 0;
    }

    public static void main(String[] args) {
        RethinkDBTestClient c = new RethinkDBTestClient();
        c.init();
        if (args.length < 1) {
            System.out.println("usage: java -jar xxx.jar get/set/cas ...");
            System.exit(1);
        }

        int result = 0;

        switch (args[0]) {
        case "get":
            result = c.get();
            break;
        case "set":
            result = c.set(Long.parseLong(args[1]));
            break;
        case "cas":
            result = c.cas(Long.parseLong(args[1]), Long.parseLong(args[2]));
            break;
        }

        System.exit(result);
    }
}
