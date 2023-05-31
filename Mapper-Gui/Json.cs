using Mapper_Gui.model;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.Json;

namespace Mapper_Gui
{
    public class Json
    {
        public static void WriteJson(IEnumerable<Button> buttons)
        {
            string json = JsonSerializer.Serialize(buttons);
            File.WriteAllText(@".\save.json", json);
        }

        public static List<Button> ReadJson()
        {
            string curFile = @".\save.json";
            if (File.Exists(curFile))
            {
                var json = File.ReadAllText(curFile);
                return JsonSerializer.Deserialize<List<Button>>(json);
            }
            return new List<Button>();
        }
    }
}
