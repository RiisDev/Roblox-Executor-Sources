import { Group, Stack, Text, Title } from "@mantine/core";
import { NextLink } from "@mantine/next";
import { IconArrowRight } from "@tabler/icons";
import { NextPage } from "next";

const Page: NextPage = () => {
  return (
    <Stack
      spacing="xs"
      align="center"
      justify="center"
      sx={{
        flexGrow: 1,
      }}
    >
      <Title size={70} variant="gradient">
        404
      </Title>
      <Text color="dimmed">Page not found &#8212; Try again!</Text>
      <Text
        component={NextLink}
        href="/"
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
