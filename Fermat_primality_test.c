#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>

//function to test the primality of n according to base a , using fermat's test
int test_fermat_base(mpz_t n, mpz_t a)
 {
  int res=1;
  mpz_t y,n1;
  mpz_inits(y,n1,NULL);
  mpz_sub_ui(n1,n,1);
  mpz_powm(y,a,n1,n);
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
 if(argc!=3)
  {
   printf("Using %s: n t\n",argv[0]);
   exit(-1);
  }
  
//taking the input 
int nx=atoi(argv[1]),tx=atoi(argv[2]);
mpz_t n;
mpz_init(n);
mpz_set_ui(n,nx);


//the random generator 
gmp_randstate_t generator ;//declaring a generator
gmp_randinit_default(generator); // Initializing the generator
gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed
//applying the test
int res=test_fermat(n,t,generator);

//printing out the output
switch (res)
 {
  case 0:
  gmp_printf("%Zd is probably prime\n",n);
  break;
  
  case 1:
  gmp_printf("%Zd is composite\n",n);
  break;
 }
mpz_clear(n);
return 0;
} 
