public class MiniJVM {
    private String name;
    private int age;
    public static void main(String[] args) {
        int sum = sum();
        System.out.println(sum);
    }

    public static int sum() {
        int a = 100;
        int b = 200;
        int c = 300;
        return (a + b) * c;
    }
}