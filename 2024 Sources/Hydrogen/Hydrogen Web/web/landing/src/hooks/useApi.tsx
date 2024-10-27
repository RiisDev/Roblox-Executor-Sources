import axios, { AxiosRequestConfig, AxiosResponse } from "axios";

export const useApi = () => {
  const fetch = async <T,>(
    config: AxiosRequestConfig
  ): Promise<AxiosResponse<T>> =>
    new Promise(async (resolve, reject) => {
      try {
        const response = await axios({
          baseURL: `https://api.${process.env.NEXT_PUBLIC_HOSTNAME}`,
          withCredentials: true,
          ...config,
        });

        resolve(response.data);
      } catch (error: any) {
        reject(error.response);
      }
    });

  return {
    fetch,
  };
};
