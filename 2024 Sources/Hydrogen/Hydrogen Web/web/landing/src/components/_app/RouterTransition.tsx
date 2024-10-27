import {
  completeNavigationProgress,
  NavigationProgress,
  startNavigationProgress,
} from "@mantine/nprogress";
import { useRouter } from "next/router";
import { FC, useEffect } from "react";

export const RouterTransition: FC = () => {
  const router = useRouter();

  useEffect(() => {
    const handleStart = (url: string) =>
      url !== router.asPath && startNavigationProgress();
    const handleComplete = () => completeNavigationProgress();

    router.events.on("routeChangeStart", handleStart);
    router.events.on("routeChangeComplete", handleComplete);
    router.events.on("routeChangeError", handleComplete);

    return () => {
      router.events.off("routeChangeStart", handleStart);
      router.events.off("routeChangeComplete", handleComplete);
      router.events.off("routeChangeError", handleComplete);
    };
  }, [router.asPath, router.events]);

  return <NavigationProgress color="#4ba9ff" />;
};
