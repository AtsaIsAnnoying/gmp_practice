#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>


//a fast way to calculate res=base^exp mod mod
void square_and_multiply(mpz_t res, mpz_t base, mpz_t exp, mpz_t mod)
{
mpz_set_ui(res,1);
int k=mpz_sizeinbase(exp,2);
for(int i=k;i>=0;i--)
{
 mpz_powm_ui (res, res , 2, mod);
 if (mpz_tstbit (exp,i))
 {
  mpz_mul (res, res, base);
  mpz_mod (res, res, mod);
 }
}
}
//function to test the primality of n according to base a , using fermat's test
int test_fermat_base(mpz_t n, mpz_t a)
 {
  int res=1;
  mpz_t y,n1;
  mpz_inits(y,n1,NULL);
  mpz_sub_ui(n1,n,1);
  square_and_multiply(y,a, n1, n);
  if(mpz_cmp_ui(y,1)==0)
   {
    res=0;
   }
  return res;
 }
 
//function will test n for n bases 
int test_fermat(mpz_t n, int t,gmp_randstate_t generator)
 {
  int res;
  mpz_t a,n1;
  mpz_inits(a,n1,NULL);
  mpz_sub_ui(n1,n,1);
  for(int i=1;i<=t;i++)
   {
    mpz_urandomm (a,generator, n1);
    if(mpz_cmp_ui(a,2)<0)
     {
      mpz_add_ui(a,a,2);
     }
    res=test_fermat_base(n,a);
   }
   return res;
  }
   
  

//main function that takes as input the number n , the base a and determine if n is probably prime or composite
int main(int argc, char *argv[])
{

//making sure we have the correct input
 if(argc!=4)
  {
   printf("Using %s: b t r\n",argv[0]);
   exit(-1);
  }
  
//taking the input 
int b=atoi(argv[1]),t=atoi(argv[2]),r=atoi(argv[3]),res,counter=0;
double dens, theo_dens;

//a=2^b-1
mpz_t a,z_n[r];
mpz_init(a);
mpz_ui_pow_ui (a, 2, b-1);

//the random generator 
gmp_randstate_t generator ;//declaring a generator
gmp_randinit_default(generator); // Initializing the generator
gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed


for(int i=0;i<r;i++)
 { 
  //declaring, initializing and generating r random integers that are exactly b bit long
  mpz_t z_n[i];
  mpz_init(z_n[i]);
  mpz_urandomb (z_n[i], generator ,b-1 ); //generate an integer n of at most k bits
  mpz_add(z_n[i],z_n[i],a);
  
  //applying the test
  res=test_fermat(z_n[i],t,generator);

  //counting the probable primes that are generated
  if(res==0)
   {
    counter++;
    gmp_printf("%Zd is probably prime\n",z_n[i]);
   }
 }

//comparing the density that we got with the theoratical one
theo_dens=1/(float)(b-1);
dens=(float)counter/r;
printf("dens=%f and theoretical density =%f\n",dens,theo_dens);
gmp_randclear (generator);//clearing the generator 
mpz_clear(a);
return 0;
} 
