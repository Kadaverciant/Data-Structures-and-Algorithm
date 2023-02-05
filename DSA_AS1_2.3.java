/*
 * Vsevolod Klyushev BS20-02
 * This is a program for task 2.3.
 * link to codeforces:
 * https://codeforces.com/group/C71Rz4W66e/contest/316517/submission/108041398
 */

/**
 * ====================================Description of code==============================================================
 * In order to solve our problem we use HashMap with keys-date and values of class DateProfit that contains total sum
 * for particular day and number of unique receipt IDs.
 *
 * At the same time DateProfit is a pair of sum and another one HashMap, lets call it idMap for simplicity.
 * That idMap contain a pair id (key) and null (value), that give us an opportunity to get number of unique date with a
 * constant time due to getSize function.
 *
 * So, we just use our maps and could easily receive an answer
 */
import java.util.Iterator;
import java.util.Objects;
import java.util.Scanner;

public class Main {
    /**
     * This function erase first char and parse the result to double
     * @param dollar
     * @return Double.parseDouble(dollar.substring(1))
     */
    private static double getPrice (String dollar) {
        return Double.parseDouble(dollar.substring(1));
    }

    public static void main(String[] args) {
        Scanner cin = new Scanner(System.in);
        int number=cin.nextInt(); cin.nextLine();
        HashMap<String,DateProfit> fullMap = new HashMap<>();
        String inputLine;
        String date, id, dollars;
        double price;

        for (int i = 0; i < number; i++) {
            inputLine = cin.nextLine();
            Scanner ssin = new Scanner(inputLine);
            date = ssin.next();
            ssin.next();
            id = ssin.next();
            dollars = ssin.next();
            price = getPrice(dollars);

            while (ssin.hasNext())
                ssin.next();

            DateProfit profit = fullMap.get(date);

            if (profit != null) {
                profit.plusProfit(price, id);
            } else {
                profit = new DateProfit(price, id);
                fullMap.put(date, profit);
            }
        }
        for (HashMap<String,DateProfit>.Pair elem:fullMap) {
            date = elem.getKey();
            price = elem.getValue().getSum();
            int numOfID = elem.getValue().getAmountOfUnicsID();
            System.out.println(String.format("%s $%.2f %d", date, price, numOfID));
        }
    }
}

/*
 * This class helps ass to store total sum and amount of unique receipt IDs for particular day
 */
class DateProfit {
    private double sum = 0.0;
    private HashMap<String,Object> idMap = new  HashMap<>();

    /**
     * It is a constructor for our class
     * @param sum
     * @param id
     */
    public DateProfit(double sum, String id) {
        this.sum = sum;
        this.idMap.put(id,null);
    }

    /**
     *This function update the total sum for particular day
     * @param cash
     * @param newID
     */
    public void plusProfit(double cash, String newID) {
        this.sum += cash;
        this.idMap.put(newID, null);
    }

    /**
     * It is getter for our sum for particular day
     * @return sum
     */
    public double getSum() {
        return sum;
    }

    /**
     * This function allow as to get amount of spe
     * @return myMap.getSize()
     */
    public int getAmountOfUnicsID() {
        return idMap.getSize();
    }
}

/**
 * This is an interface for Map
 * @param <K>
 * @param <V>
 */
interface Map<K,V> {
    void put(K key, V value);
    V get(K key);
    int getSize();
}

/**
 * This is an implementation of our Map using Hash codes
 * @param <K>
 * @param <V>
 */
class HashMap<K,V> implements Map<K,V>, Iterable<HashMap<K,V>.Pair> {
    private ArrayList<Pair>[] data;
    private int size = 0;

    /**
     * This class is analog for pairs in c++, but using names for our properties <key, value>
     */
    public class Pair {
        private final K key;
        private final V value;

        /**
         * It is a constructor for our class
         * @param key
         * @param value
         */
        public Pair(K key,V value) {
            this.key = key;
            this.value = value;
        }

        /**
         * It is a getter for first field (key) of our class
         * @return key
         */
        public K getKey() {
            return key;
        }

        /**
         * It is a getter for second field (value) of our class
         * @return value
         */
        public V getValue() {
            return value;
        }

        /**
         * Hash function for our key values
         * @param dataSize
         * @return Math.abs(Objects.hash(key)) % dataSize
         */
        private int hashCode(int dataSize) {
            return (Math.abs(Objects.hash(key)) % dataSize);
        }
    }

    /**
     * It is an constructor for our map
     */
    public HashMap() {
        data = new ArrayList[8];
        for (int i = 0; i < data.length; i++) {
            data[i] = new ArrayList<>();
        }
    }

    /**
     * This function resize our Array List of pairs and recalculating Hash codes for them
     */
    private void resize() {
        ArrayList<Pair>[] temp = new ArrayList[data.length*2];
        for (int i = 0; i < temp.length; i++) {
            temp[i] = new ArrayList<>();
        }

        for (ArrayList<Pair> chain : data) {
            for (int j = 0; j < chain.getSize(); j++) {
                Pair cell = chain.get(j);
                int index = cell.hashCode(temp.length);
                temp[index].pushBack(cell);
            }
        }
        data = temp;
    }

    @Override
    public Iterator<Pair> iterator() {
        return new Iterator<Pair>() {
            private int num = 0;
            private int indexOfData = 0;
            private int indexOfChain = 0;
            @Override
            public boolean hasNext() {
                return num < size;
            }

            @Override
            public Pair next() {
                if (indexOfChain == data[indexOfData].getSize()) {
                    indexOfData++;
                    while (data[indexOfData].isEmpty())
                        indexOfData++;
                    indexOfChain = 0;
                }
                num++;
                return data[indexOfData].get(indexOfChain++);
            }
        };
    }

    /**
     * This function put/update a pair into our Array List with index that depends on key
     * @param key
     * @param value
     */
    @Override
    public void put(K key, V value) {
        if (size == data.length) {
            resize();
        }
        Pair cell = new Pair(key,value);
        int index = cell.hashCode(data.length);
        ArrayList<Pair> chain = data[index];
        for(int i=0; i < chain.getSize();i++) {
            if (key.equals(chain.get(i).key)) {
                data[index].set(i, new Pair(key, value));
                return;
            }
        }
        data[index].pushBack(cell);
        size++;
    }

    /**
     * This function return a value that corresponds to special key
     * @param key
     * @return temp.get(i).value
     */
    @Override
    public V get(K key) {
        Pair cell = new Pair(key, null);
        int index = cell.hashCode(data.length);
        ArrayList<Pair> temp = data[index];
        for (int i = 0; i < temp.getSize(); i++) {
            if (temp.get(i).key.equals(key)) {
                return temp.get(i).value;
            }
        }
        return null;
    }

    /**
     *This function returns the size of our map
     * @return size
     */
    @Override
    public int getSize() {
        return size;
    }
}

/**
 * This is an interface for List
 * @param <T>
 */
interface List<T> {
    void pushBack(T elem);
    T get(int i);
    void set(int i, T elem);
    int getSize();
    boolean isEmpty();
}

/**
 * This is implementation of List using arrays
 * @param <T>
 */
class ArrayList<T> implements List<T> {

    private int size = 0;
    private T[] data;

    /**
     * This is a constructor for our class
     */
    ArrayList() {
        data = (T[]) (new Object[8]);
    }

    /**
     * This function resize our data
     */
    private void resize() {
        T[] temp = (T[]) (new Object[data.length*2]);
        for (int i=0; i < data.length; i++) {
            temp[i] = data[i];
        }
        data = temp;
    }

    /**
     * This function put new element at the end of the list
     * @param elem
     */
    @Override
    public void pushBack(T elem) {
        if (size == data.length) {
            resize();
        }
        data[size++] = elem;
    }

    /**
     * This function returns an element with index i
     * @param i
     * @return data[i]
     */
    @Override
    public T get(int i) {
        return data[i];
    }

    /**
     * This function set element in index i
     * @param i
     * @param elem
     */
    @Override
    public void set(int i, T elem) {
        data[i] = elem;
    }

    /**
     * This function returns the size of the Array list
     * @return
     */
    @Override
    public int getSize() {
        return size;
    }

    /**
     * This function check if our list is empty
     * @return size == 0
     */
    @Override
    public boolean isEmpty() {
        return size == 0;
    }
}

