public class MiniJVM implements DarrenRunnable{

    private static final Student student = new Student();

    @Override
    public void run() {
        for(int i = 0; i < 10000; i++) {
            int age = student.getAge();
            age += 1;
            student.setAge(age);
        }
    }

    public static void main(String[] args){
        /*int a = 12;
        int b = 13;
        int sum = sum(a, b);
        Print.println(sum);

        Student stu = new Student();
        stu.setAge(30);
        stu.printAge();
        int age = stu.getAge();
        Print.println(age);*/

        DarrenThread thread1 = new DarrenThread(new MiniJVM());
        DarrenThread thread2 = new DarrenThread(new MiniJVM());
        thread1.start();
        thread2.start();
        DarrenThread.sleepNanos(1000);
        Print.println("age = " + student.getAge());
    }

    public static int sum(int number1, int number2) {
        return number1 + number2;
    }
}