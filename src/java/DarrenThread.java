public class DarrenThread {
    private DarrenRunnable mRunnable;

    public DarrenThread(DarrenRunnable runnable) {
        this.mRunnable = runnable;
    }

    public void run() {
        DarrenRunnable task = mRunnable;
        if (task != null) {
            task.run();
        }
    }

    public native void start();

    public static native void sleepNanos(long nanos);
}
