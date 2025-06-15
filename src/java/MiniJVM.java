public class MiniJVM {

    public static void main(String[] args) {
        int a = 12;
        int b = 13;
        int sum = sum(a, b);
        Print.println(sum);

        Student stu = new Student();
        stu.setAge(30);
        stu.printAge();
        int age = stu.getAge();
        Print.println(age);
    }

    public static int sum(int number1, int number2) {
        return number1 + number2;
    }
}