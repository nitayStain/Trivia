using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    class Protocol
    {
        // parses a code and a given object according to the protocol
        public static byte[] SerializeMessage<T>(RequestCode code, T data)
        {
            string jsonPayload = data != null ? JsonSerializer.Serialize(data) : "";  // get the json payload as string
            byte[] lengthInBytes = BitConverter.GetBytes(jsonPayload.Length); // get its length as bytes (4 sized array)
            byte[] payload = Encoding.ASCII.GetBytes(jsonPayload); // convert json to bytes

            byte[] buffer = new byte[5 + payload.Length]; // 5 -> 1 byte (code) + 4 bytes (int size)
            buffer[0] = (byte)code;
            lengthInBytes.CopyTo(buffer, 1);
            payload.CopyTo(buffer, 5);

            return buffer;
        }

        public static T DeserializeMessage<T>(string json)
        {
            return JsonSerializer.Deserialize<T>(json);
        }

        public static byte GetResponseCode(Stream stream)
        {
            int byt = stream.ReadByte();

            return (byte)byt;
        }

        public static string GetString(Stream stream)
        {
            byte[] sizeBytes = GetBytes(stream, sizeof(int));
            int size = BitConverter.ToInt32(sizeBytes);

            byte[] buffer = GetBytes(stream, size);
            return Encoding.ASCII.GetString(buffer);
        }

        public static T GetPayload<T>(Stream stream)
        {
            return DeserializeMessage<T>(GetString(stream));
        }

        private static byte[] GetBytes(Stream stream, int size)
        {
            byte[] buffer = new byte[size];

            stream.Read(buffer, 0, size);

            return buffer;
        }
    }
}
