using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public class TriviaManager
    {
        private ThreadsafeClient _client = new ThreadsafeClient();

        public void Init(string ip, ushort port)
        {
            _client.Initialize(ip, port);
        }

        public void Shutdown()
        {
            _client.Disconnect();
        }

        public bool Login(string username, string password, out string error)
        {
            App.Username = username;

            error = "Invalid error happened.";
            LoginRequest request = new LoginRequest();
            request.username = username;
            request.password = password;

            var payload = Protocol.SerializeMessage(RequestCode.LOGIN_REQUEST, request);

            _client.ClientStream.Write(payload);

            return ReadCurrentTask<LoginResponse>(out error);
        }

        public bool Signup(string username, string password, string email, out string error)
        {
            App.Username = username;

            error = "Invalid error happened.";
            SignupRequest request = new SignupRequest();
            request.username = username;
            request.password = password;
            request.email = email;

            var payload = Protocol.SerializeMessage(RequestCode.SIGNUP_REQUEST, request);

            _client.ClientStream.Write(payload);

            return ReadCurrentTask<SignupResponse>(out error);
        }

        public void GetRooms(Action<RoomData[]> onDataReceived)
        {
            EmptyRequest req = new EmptyRequest();

            var payload = Protocol.SerializeMessage(RequestCode.GET_ROOMS_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if(code == ResponseCode.ERROR_RESPONSE)
                {
                    // empty socket buffer
                    _client.Read<ErrorResponse>();
                    onDataReceived.Invoke(null);
                }
                else
                {
                    var data = Protocol.DeserializeMessage<GetRoomsResponse>(payload);

                    onDataReceived.Invoke(
                        data.rooms
                    );
                }
            });
        }

        public bool JoinRoom(string id, out string error)
        {
            error = "";
            JoinRoomRequest req = new JoinRoomRequest();
            req.roomId = id;

            var payload = Protocol.SerializeMessage(RequestCode.JOIN_ROOM_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<JoinRoomResponse>(out error);
        }

        public bool CreateRoom(string roomName, uint maxPlayers, uint answerTimeout, out string error)
        {
            error = "";
            CreateRoomRequest req = new CreateRoomRequest();
            req.roomName = roomName;
            req.maxUsers = maxPlayers;
            req.answerTimeout = answerTimeout;

            var payload = Protocol.SerializeMessage(RequestCode.CREATE_ROOM_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<CreateRoomResponse>(out error);
        }
        
        public void GetRoomData(Action<GetRoomStateResponse> callback)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage(RequestCode.GET_ROOM_STATE_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if(code == ResponseCode.ERROR_RESPONSE)
                {
                    Trace.WriteLine(payload);
                    callback.Invoke(null);
                }
                else
                {
                    callback.Invoke(Protocol.DeserializeMessage<GetRoomStateResponse>(payload));
                }
            });
        }

        private bool ReadCurrentTask<T>(out string error) where T : IResponse
        {
            error = "";
            var code = _client.GetCurrentResponse();
            if (code == ResponseCode.ERROR_RESPONSE)
            {
                var resp = _client.Read<ErrorResponse>();
                if (resp != null)
                {
                    error = resp.message;
                }

                return false;
            }
            else
            {
                _client.Read<T>();
                error = "";
                return true;
            }
        }

        public void GetStats(Action<string[]> onStatsReceived)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage(RequestCode.GET_PERSONAL_STATISTICS_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if (code == ResponseCode.ERROR_RESPONSE)
                {
                    // empty socket buffer
                    _client.Read<ErrorResponse>();
                    onStatsReceived.Invoke(null);
                }
                else
                {
                    var data = Protocol.DeserializeMessage<GetPersonalStatsResponse>(payload);
                    onStatsReceived.Invoke(data.statistics);
                }
            });
        }

        public void GetHighScores(Action<string[]> onScoresReceived)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage<EmptyRequest>(RequestCode.GET_HIGH_SCORE_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if (code == ResponseCode.ERROR_RESPONSE)
                {
                    _client.Read<ErrorResponse>();
                    onScoresReceived.Invoke(null);
                }
                else
                {
                    var data = Protocol.DeserializeMessage<GetHighScoreResponse>(payload);
                    onScoresReceived.Invoke(data.statistics);
                }
            });
        }

        public bool StartGame(out string error)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage<EmptyRequest>(RequestCode.START_GAME_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<StartGameResponse>(out error);
        }

        public bool CloseRoom(out string error)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage<EmptyRequest>(RequestCode.CLOSE_ROOM_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<CloseRoomResponse>(out error);
        }

        public bool LeaveRoom(out string error)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage<EmptyRequest>(RequestCode.LEAVE_ROOM_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<LeaveRoomResponse>(out error);
        }

        public bool CheckAnswer(uint answerId)
        {
            SubmitAnswerRequest req = new SubmitAnswerRequest();
            req.answerId = answerId;
            var payload = Protocol.SerializeMessage(RequestCode.SUBMIT_ANSWER_REQUEST, req);
            _client.ClientStream.Write(payload);

            var data = _client.Read<SubmitAnswerResponse>();
            return data.correctAnsId == answerId;
        }

        public void GetQuestion(Action<getQuestionResponse> callback)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage(RequestCode.GET_QUESTION_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if (code == ResponseCode.ERROR_RESPONSE)
                {
                    _client.Read<ErrorResponse>();
                    callback.Invoke(null);
                }
                else
                {
                    callback.Invoke(Protocol.DeserializeMessage<getQuestionResponse>(payload));
                }
            });
        }

        public bool LeaveGame(out string error)
        {
            error = "";
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage(RequestCode.LEAVE_GAME_REQUEST, req);
            _client.ClientStream.Write(payload);

            return ReadCurrentTask<LeaveGameResponse>(out error);
        }

        public void getGameResults(Action<getGameResultsResponse> callback)
        {
            EmptyRequest req = new EmptyRequest();
            var payload = Protocol.SerializeMessage(RequestCode.GET_GAME_RESULTS_REQUEST, req);
            _client.ClientStream.Write(payload);

            _client.HandleData((ResponseCode code, string payload) =>
            {
                if (code == ResponseCode.ERROR_RESPONSE)
                {
                    _client.Read<ErrorResponse>();
                    callback.Invoke(null);
                }
                else
                {
                    callback.Invoke(Protocol.DeserializeMessage<getGameResultsResponse>(payload));
                }
            });
        }
    }
}
