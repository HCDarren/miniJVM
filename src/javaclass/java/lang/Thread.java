public class Thread {
    private Runnable mRunnable;

    public Thread(Runnable runnable) {
        this.mRunnable = runnable;
    }

    public void run() {
        Runnable task = mRunnable;
        if (task != null) {
            task.run();
        }
    }

    public native void start();

    public static native void sleep(long millisconds);
}
