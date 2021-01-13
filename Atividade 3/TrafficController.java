// Desenvolvido por: Angelina Gomes e Felipe Amorim

import java.util.concurrent.locks.ReentrantLock;

public class TrafficController {

    private ReentrantLock mutex = new ReentrantLock();

    public void lockMutex() {
        mutex.lock();
    }

    public void unlockMutex() {
        mutex.unlock();
    }

}
