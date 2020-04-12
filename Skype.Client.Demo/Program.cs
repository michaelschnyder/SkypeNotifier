﻿using System;
using Skype.Client.CefSharp.OffScreen;

namespace Skype.Client.Demo
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 2)
            {
                Console.WriteLine("Creating new instance of client");
                var app = new SkypeCefOffScreenClient();

                app.StatusChanged += OnAppOnStatusChanged;
                app.IncomingCall += (sender, eventArgs) => Console.WriteLine(eventArgs);
                app.CallStatusChanged += (sender, eventArgs) => Console.WriteLine(eventArgs);
                app.MessageReceived += (sender, eventArgs) => Console.WriteLine(eventArgs);

                Console.WriteLine("Starting authentication. This might take a few seconds.");

                app.Login(args[0], args[1]);

                Console.ReadKey();
            }
            else
            {
                Console.WriteLine("Parameters mismatch");
            }
        }

        private static void OnAppOnStatusChanged(object sender, StatusChangedEventArgs eventArgs)
        {
            if (eventArgs.New != AppStatus.Connected)
            {
                Console.WriteLine($"Client status: {eventArgs.New}");
            }
            else
            {
                Console.WriteLine("Ready! :). You will see incoming messages and calls on this command line shell. Press any key to exit.");
            }
        }
    }
}