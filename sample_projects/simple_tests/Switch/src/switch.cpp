int main()
{
   int a = 1;

   switch( a )
   {
      case 0:
         a++;
         break;
      case 1:
         a--;
      case 2:
         a++;
         break;
      default:
         break;
    }

   return 0;
}
