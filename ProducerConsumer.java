import java.util.concurrent.Semaphore;

class ProducerConsumer {
    static final int BUFFER_SIZE = 5;
    static int[] buffer = new int[BUFFER_SIZE];
    static int in = 0, out = 0;
    static Semaphore empty = new Semaphore(BUFFER_SIZE);
    static Semaphore full = new Semaphore(0);
    static Semaphore mutex = new Semaphore(1);

    static class Producer extends Thread {
        public void run() {
            while (true) {
                try {
                    int item = (int) (Math.random() * 100);

                    empty.acquire();
                    mutex.acquire();
                    buffer[in] = item;
                    System.out.println("Produced: " + item);
                    in = (in + 1) % BUFFER_SIZE;
                    mutex.release();
                    full.release();

                    Thread.sleep((int) (Math.random() * 1000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    static class Consumer extends Thread {
        public void run() {
            while (true) {
                try {
                    full.acquire();
                    mutex.acquire();
                    int item = buffer[out];
                    System.out.println("Consumed: " + item);
                    out = (out + 1) % BUFFER_SIZE;
                    mutex.release();
                    empty.release();

                    Thread.sleep((int) (Math.random() * 1000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        Producer producer = new Producer();
        Consumer consumer = new Consumer();
        producer.start();
        consumer.start();
    }
}
