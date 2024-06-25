using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Diagnostics;

namespace Client
{
    internal class Connection
    {
        // singleton
        private static Connection m_instance;

        // the client socket
        private TcpClient m_client;

        public bool Connected {  
            get
            {
                return m_client == null ? false : m_client.Connected;
            }
        }

        /// <summary>
        /// The buffer of the socket - manages writing and reading
        /// </summary>
        public Stream ClientStream { get; private set; }

        // singleton function
        public static Connection GetInstance()
        {
            if(m_instance == null)
                m_instance = new Connection();

            return m_instance;
        }

        // disconnects from the socket, if logged in -> sends logout request too.
        public void Disconnect()
        {
            if(m_client != null) 
            {
                if(App.IsAuthenticated)
                {
                    Send(RequestCode.LOGOUT_REQUEST, null);
                }

                m_client.Close();
            }

            m_client = null;
            ClientStream = null;
        }

        // creates a new socket that connects to the ip and the port
        public void Initialize(string ip, ushort port)
        {
            m_client = new TcpClient(ip, port);
            ClientStream = m_client.GetStream();
        }
        
        // sends a serialized message with code and a request
        public virtual void Send(RequestCode code, IRequest data)
        {
            ClientStream.Write(Protocol.SerializeMessage(code, data));
        }

        // returns the first byte from the buffer (the current message code)
        public ResponseCode GetCurrentResponse()
        {
            return (ResponseCode) Protocol.GetResponseCode(ClientStream);
        }

        // returns the deserialized payload
        public T Read<T>()
        {
            var res = Protocol.GetPayload<T>(ClientStream);

            return res;
        }
    }
}
