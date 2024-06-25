using Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public enum ResponseCode
    {
        /* Login-type */
        LOGIN_RESPONSE = 0,
        SIGNUP_RESPONSE,

        /* menu responses */
        CREATE_ROOM_RESPONSE = 10,
        JOIN_ROOM_RESPONSE,
        GET_ROOMS_RESPONSE,
        GET_PLAYERS_IN_ROOM_RESPONSE,
        GET_HIGH_SCORE_RESPONSE,
        GET_PERSONAL_STATS_RESPONSE,
        LOGOUT_RESPONSE,

        /* room admin responses */
        CLOSE_ROOM_RESPONSE = 20,
        START_GAME_RESPONSE,
        /* room member responses */
        GET_ROOM_STATE_RESPONSE = 25,
        LEAVE_ROOM_RESPONSE,

        /* game responses */
        LEAVE_GAME_RESPONSE = 30,
        SUBMIT_ANSWER_RESPONSE,
        GET_GAME_RESULTS_RESPONSE,
        GET_QUESTION_RESPONSE,

        /* other */
        ERROR_RESPONSE = 99,
    }

    public class RoomData
    {
        public string name { get; set; }
        public string id { get; set; }
        public uint maxPlayers { get; set; }
        public uint isActive { get; set; }
    }

    public class PlayerResults
    {
        public string username { get; set; }
        public uint correctAnsCount { get; set; }
        public uint wrongAnsCount { get; set; }
        public uint avgAnsTime { get; set; }
    }

    public class ErrorResponse
    {
        public string message { get; set; }
    }

    public interface IResponse
    {
        uint code { get; set; }
    }


    public class LoginResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class LogoutResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class SignupResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class CreateRoomResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class JoinRoomResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class GetRoomsResponse : IResponse
    {
        public uint code { get; set; }
        public RoomData[] rooms { get; set; }
    }

    public class GetPlayersInRoomResponse : IResponse
    {
        public uint code { get; set; }
        public string[] players { get; set; }
    }

    public class GetHighScoreResponse : IResponse
    {
        public uint code { get; set; }
        public string[] statistics { get; set; }
    }

    public class GetPersonalStatsResponse : IResponse
    {
        public uint code { get; set; }
        public string[] statistics { get; set; }
    }

    public class GetRoomStateResponse : IResponse
    {
        public uint code { get; set; }
        public bool hasGameBegun { get; set; }
        public string[] players { get; set; }
        public uint questionCount { get; set; }
        public uint answerTimeOut { get; set; }
    };

    public class CloseRoomResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class StartGameResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class LeaveRoomResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class LeaveGameResponse : IResponse
    {
        public uint code { get; set; }
    }

    public class SubmitAnswerResponse : IResponse
    {
        public uint code { get; set; }
        public uint correctAnsId { get; set; }
    }

    public class getGameResultsResponse : IResponse
    {
        public uint code { get; set; }
        public uint success { get; set; }
        public PlayerResults[] results { get; set; }
    }

    public class getQuestionResponse : IResponse
    {
        public uint code { get; set; }
        public uint success { get; set; }
        public string question { get; set; }
        public Dictionary<uint, string> answers { get; set; }
    }
}
