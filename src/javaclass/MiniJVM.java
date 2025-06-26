public class MiniJVM implements Runnable {

    private static final Student student = new Student();

    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            int age = student.getAge();
            age += 1;
            student.setAge(age);
            student.printAge();
        }
    }

    public static void main(String[] args) throws Exception{
        /*
        int a = 12;
        int b = 13;
        int sum = sum(a, b);
        Print.println(sum);
          
        Student stu = new Student();
        stu.setAge(30);
        stu.printAge();
        int age = stu.getAge();
        Print.println(age);
        Thread.sleep(5000);
        Print.println(student.getAge());
        try{
            testException();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }
        */
        Thread thread1 = new Thread(new MiniJVM());
        Thread thread2 = new Thread(new MiniJVM());
        //thread1.start();
        //thread2.start();

        testGC();
        Student student = new Student();
        System.gc();
        Thread.sleep(5000);
        student.setAge(100);
        student.printAge();
        Thread.sleep(30000);
    }

    public static void testGC() {
        for(int i = 0; i < 10000; i++) {
            Student student = new Student();
            student.getAge();
        }
    }

    public static int sum(int number1, int number2){
        return number1 + number2;
    }

    public static void testException() throws NullPointerException{
        throw new NullPointerException();
    }
}