/**
 * Trivial an direct algorithm.
 * Not optimized. Should be used only for UnitTesting.
 *
 * @author mayton
 */
public class prime {

    public static void printf(String s,Object ...arg){
        System.out.printf(s,arg);
    }

    public static void err(String s,Object ...arg){
        System.err.printf(s, arg);
    }

    public static void main(String[] args)
    {
        if (args.length==0){
            err("\nPrime generator 1.0 (Java). Written by mayton in Java 8\n");
            err("\nUsage: java -jar prime.jar [endprime]\n");
            return;
        }
        String arg0 = args[0];
        if (arg0.compareTo("2147483647")>0){
            err("Maximum prime is 2147483647!");
            return ;
        }
        int end = Integer.valueOf(arg0);
        if (end<2){
            err("Minimum prime is 2!");
            return ;
        }
        long time_begin=System.currentTimeMillis();
        System.out.println(2);
        int primes=1;


        for(int c=3;c<=end;c+=2)  {
            int ub= 1 + (int)Math.sqrt((double)c);
            boolean isprime=true;
            for(int i=2;i<=ub;i++)
            {
                if ((c%i)==0) {
                    isprime=false;
                    break;
                }
            }
            if (isprime) {
                primes++;
                printf("%d\n", c);
            }
        }
        err("Primes detected : %d\n",primes);
        err("Elapsed time    : %d\n sec",(System.currentTimeMillis()-time_begin)/1000);
    }

}
