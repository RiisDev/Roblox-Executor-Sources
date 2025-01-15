using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace Atlantis.Classes
{
    public class HttpService
    {
        private readonly HttpClient _client;

        public HttpService()
        {
            HttpClientHandler handler = new HttpClientHandler
            {
                AutomaticDecompression = DecompressionMethods.GZip | DecompressionMethods.Deflate
            };

            _client = new HttpClient();
        }

        //public async Task<string> GetAsync(string uri)
        //{
        //    try
        //    {
        //        using (HttpResponseMessage response = new HttpResponseMessage())
        //        {
        //            await _client.GetAsync(uri);
        //            return await response.Content.ReadAsStringAsync();

        //        }
        //    }
        //    catch (Exception ex)
        //    {
        //        return null;
        //    }
        //}

        public async Task<string> PostAsync(string uri, string data, string contentType)
        {
            using (HttpContent content = new StringContent(data, Encoding.UTF8, contentType))
            {
                HttpRequestMessage requestMessage = new HttpRequestMessage()
                {
                    Content = content,
                    Method = HttpMethod.Post,
                    RequestUri = new Uri(uri)
                };

                using (HttpResponseMessage response = await _client.SendAsync(requestMessage))
                {
                    return await response.Content.ReadAsStringAsync();

                }

            }

          
        }
    }
}
