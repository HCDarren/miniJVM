public class MiniJVM {
    private String name;
    private int age;
    public static void main(String[] args) {
        int a = 12;
        int b = 13;
        int sum = sum(a, b);
        System.out.println(sum);
    }

    public static int sum(int number1, int number2) {
        return number1 + number2;
    }
}