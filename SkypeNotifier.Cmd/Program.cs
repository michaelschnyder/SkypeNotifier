using System;
using System.Linq;
using System.Threading.Tasks;
using Skype.Client;
using Skype.Client.CefSharp.OffScreen;
using Telegram.Bot;
using Telegram.Bot.Types.Enums;

namespace SkypeNotifier.Cmd
{
    internal class Program
    {
        private static TelegramBotClient _bot;
        private static string _telegramTarget;
        private static SkypeCefOffScreenClient _client;

        static async Task Main(string[] args)
        {
            if (args.Length >= 3)
            {
                Console.WriteLine("Configuring Telegram");
                await ConfigureTelegram(args);

                Console.WriteLine("Creating Client");
                InitializeClient(args);

                Console.ReadKey();
            }
            else
            {
                Console.WriteLine("Parameters mismatch");
            }
        }

        private static void ClientOnIncomingCall(object sender, CallEventArgs e)
        {
            try
            {
                Console.WriteLine($"Incoming call. Sending notification to {_telegramTarget}");
                var result = _bot.SendTextMessageAsync(_telegramTarget, $"New Call from {e.CallerName}").Result;
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
            }
        }

        private static void ClientOnStatusChanged(object sender, StatusChangedEventArgs e)
        {
            if (e.New == AppStatus.Ready)
            {
                Console.WriteLine($"Connected as '{_client.Me.DisplayName}'");
            }
        }

        private static void InitializeClient(string[] args)
        {
            _client = new SkypeCefOffScreenClient();
            _client.StatusChanged += ClientOnStatusChanged;
            _client.IncomingCall += ClientOnIncomingCall;
            _client.CallStatusChanged += (sender, eventArgs) => Console.WriteLine(eventArgs);
            _client.MessageReceived += (sender, eventArgs) => Console.WriteLine(eventArgs);

            Console.WriteLine("Logging in");
            _client.Login(args[0], args[1]);
        }

        private static async Task ConfigureTelegram(string[] args)
        {
            _bot = new TelegramBotClient(args[2]);

            var me = await _bot.GetMeAsync();
            Console.WriteLine($"Telegram Bot started. I am user {me.Id} and my name is {me.FirstName}.");

            var updates = await _bot.GetUpdatesAsync();
            _telegramTarget = "";
            if (args.Length == 4)
            {
                _telegramTarget = args[3];
            }

            if (string.IsNullOrWhiteSpace(_telegramTarget) || !updates.Any(u => u.Message?.Chat?.Id.ToString() == _telegramTarget || u.ChannelPost?.Chat?.Id.ToString() == _telegramTarget))
            {
                var chats = updates.Where(u => u.Message != null && u.Message.Chat.Type == ChatType.Private);
                foreach (var update in chats)
                {
                    Console.WriteLine($"Bot has access to chat with '{update.Message.Chat.FirstName}', id: {update.Message.Chat.Id}");
                }

                var groups = updates.Where(u => u.Message != null && u.Message.Chat.Type == ChatType.Group);
                foreach (var update in groups)
                {
                    Console.WriteLine($"Bot has access to group chat named '{update.Message.Chat.Title}', id: {update.Message.Chat.Id}");
                }

                var channels = updates.Where(u => u.ChannelPost != null);
                foreach (var update in channels)
                {
                    Console.WriteLine($"Bot has access to channel '{update.ChannelPost.Chat.Title}' with id: {update.ChannelPost.Chat.Id}");
                }
            }
            else
            {
                Console.WriteLine("Unknown or not specified target for telegram notifications.");
            }
        }

    }
}
