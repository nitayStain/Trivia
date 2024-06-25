using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    public enum RequestCode
    {
	    /* login requests */
	    LOGIN_REQUEST = 0,
	    SIGNUP_REQUEST,

        /* room requests */
        CREATE_ROOM_REQUEST = 10,
        GET_PLAYERS_IN_ROOM_REQUEST,
        JOIN_ROOM_REQUEST,
        GET_HIGH_SCORE_REQUEST,
        GET_PERSONAL_STATISTICS_REQUEST,
        GET_ROOMS_REQUEST,
        LOGOUT_REQUEST,

        /* room admin requests */
        CLOSE_ROOM_REQUEST = 20,
        START_GAME_REQUEST,
        /* room member requests */
        GET_ROOM_STATE_REQUEST = 25,
        LEAVE_ROOM_REQUEST,

        /* game requests */
        LEAVE_GAME_REQUEST = 30,
        SUBMIT_ANSWER_REQUEST,
        GET_GAME_RESULTS_REQUEST,
        GET_QUESTION_REQUEST,

        /* other */
        EXIT = 99
    }

    public abstract class IRequest { }

    // attached to get rooms, logout and statistics
    public class EmptyRequest : IRequest { }

    /* request payloads */
    public class LoginRequest : IRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }

    public class SignupRequest : IRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }

    public class CreateRoomRequest : IRequest
    {
        public string roomName { get; set; }
        public uint maxUsers { get; set; }
        public uint questionCount { get; set; }
        public uint answerTimeout { get; set; }
}

    public class JoinRoomRequest : IRequest
    {
        public string roomId { get; set; }
    }

    public class GetPlayersInRoomRequest : IRequest
    {
        public string roomId { get; set; }
    }

    public class SubmitAnswerRequest : IRequest
    {
        public uint answerId { get; set; }
    }
}
