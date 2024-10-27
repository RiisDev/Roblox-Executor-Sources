import axios, { AxiosRequestConfig, AxiosResponse } from "axios";
import { useRouter } from "next/router";

export const useApi = () => {
  const router = useRouter();

  const fetch = async <T,>(
    config: AxiosRequestConfig
  ): Promise<AxiosResponse<T>> =>
    new Promise(async (resolve, reject) => {
      try {
        const response = await axios({
          baseURL: `https://api.${process.env.NEXT_PUBLIC_HOSTNAME}`,
          withCredentials: true,
          headers: {
            Authorization:
              router.isReady && router.query.apiKey
                ? router.query.apiKey
                : undefined,
          },
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
