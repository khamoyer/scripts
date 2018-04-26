using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace BruteMan
{
    public class Product
    {
        public int Id { get; set; }
        public String Name {get; set;}
    }

    class Program
    {
        static async Task Main(string[] args)
        {
            await ActAsync();
            //Act();
            //ActBig();
        }

        private static async Task ActAsync()
        {
            Console.WriteLine( $"hello async");
            await Task.Delay(1);
            await ActAsync();
        }

        private static void Act()
        {
            Console.WriteLine( $"hello"); 
            int[] tab = new int[100];
            Act();
        }

        private static void ActBig()
        {
            Console.WriteLine( $"hello big"); 
            int [] tab = new int[1000000];
            ActBig();
        }
    }
}
