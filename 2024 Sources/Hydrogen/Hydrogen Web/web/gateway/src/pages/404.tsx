import { Alert, Group, Stack, Text, Title } from "@mantine/core";
import { IconAlertCircle, IconArrowRight } from "@tabler/icons";
import { NextPage } from "next";
import { useRouter } from "next/router";
import { useMemo } from "react";

const errors: { [key: string]: string } = {
  STATE_NOT_FOUND:
    "A valid state could not be located on your session, please copy your gateway URL from in-game and try again.",
  HWID_NOT_PROVIDED:
    "HWID could not be found in route parameters, please copy your gateway URL from in-game and try again.",
};

const Page: NextPage = () => {
  const router = useRouter();
  const query = useMemo(() => {
    const queryParamsStr = router.asPath.split("?").slice(1).join("");
    const urlSearchParams = new URLSearchParams(queryParamsStr);
    const params = Object.fromEntries(urlSearchParams.entries());

    return params;
  }, [router.asPath]);

  return (
    <Stack
      spacing="xs"
      align="center"
      justify="center"
      sx={{
        flexGrow: 1,
      }}
    >
      {query.error && errors[query.error] ? (
        <>
          <Alert
            icon={<IconAlertCircle size={32} />}
            title="Oops, something went wrong!"
            color="red"
          >
            {errors[query.error]}
          </Alert>
        </>
      ) : (
        <>
          <Title size={70} variant="gradient">
            404
          </Title>
        </>
      )}
      <Text color="dimmed">
        {query.token
          ? ` Error token: ${query.token}`
          : " Page not found — Try again!"}
      </Text>
      <Text
        component="a"
        href={`https://${process.env.NEXT_PUBLIC_HOSTNAME}`}
        sx={(theme) => ({
          fontSize: 14,
          fontWeight: 600,
          color: theme.white,
          ":hover": {
            color: theme.colors.dark[0],
          },
        })}
      >
        <Group spacing={5}>
          Or Go Home
          <IconArrowRight size={17} />
        </Group>
      </Text>
    </Stack>
  );
};

export default Page;
