using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace Atlantis.Classes
{
    public static class r
    {
        public static List<Script> rr;
    }

    public class ScriptBlox
    {
        private readonly HttpService _httpService;
        private const string FETCH_SCRIPT_URL = "https://scriptblox.com/api/script/fetch?page=1";
        //private const string SEARCH_SCRIPT_URL = "https://scriptblox.com/api/script/fetch?page=1";

        public ScriptBlox()
        {
            _httpService = new HttpService();
        }

        public async void FetchPopularScripts()
        {
           // string result = await _httpService.GetAsync(FETCH_SCRIPT_URL);
           // Root deserialized = JsonConvert.DeserializeObject<Root>(result);

            //r.rr = deserialized.result.scripts;
        }

        //public List<Script> FetchSearchedScripts(string search)
        //{
        //    string result =  _httpService.GetAsync(search);
        //    Root deserialized = JsonConvert.DeserializeObject<Root>(result);

        //    return deserialized.result.scripts;
        //}
    }

    public class Game
    {
        public object gameId { get; set; }
        public string name { get; set; }
        public string imageUrl { get; set; }
    }

    public class Owner
    {
        public string _id { get; set; }
        public string username { get; set; }
        public bool verified { get; set; }
        public string role { get; set; }
        public bool isBanned { get; set; }
        public string profilePicture { get; set; }
        public DateTime createdAt { get; set; }
        public string status { get; set; }
        public string id { get; set; }
    }

    public class Result
    {
        public int totalPages { get; set; }
        public int nextPage { get; set; }
        public int max { get; set; }
        public List<Script> scripts { get; set; }
    }

    public class Root
    {
        public Result result { get; set; }
    }

    public class Script
    {
        public string _id { get; set; }
        public string title { get; set; }
        public Game game { get; set; }
        public Owner owner { get; set; }
        public string slug { get; set; }
        public bool verified { get; set; }
        public bool key { get; set; }
        public int views { get; set; }
        public string scriptType { get; set; }
        public bool isPatched { get; set; }
        public string visibility { get; set; }
        public DateTime createdAt { get; set; }
        public int likeCount { get; set; }
        public int dislikeCount { get; set; }
        public string id { get; set; }
    }


}
