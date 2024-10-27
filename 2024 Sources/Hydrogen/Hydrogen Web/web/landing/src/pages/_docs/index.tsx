import { Container, Group } from "@mantine/core";
import { NextPage } from "next";

const Page: NextPage = () => {
  return (
    <Container
      sx={{
        flexGrow: 1,
      }}
    >
      <Group position="apart">
        {/* <Stack spacing={0}>
          <Title
            size={30}
            sx={{
              fontWeight: 700,
            }}
          >
            Documentation
          </Title>
          <Text color="dimmed" size="md">
            Explore all of our hand-written guides for making the ideal
            codebase,
            <br />
            Or check out the{" "}
            <Text
              component="span"
              sx={(theme) => ({
                cursor: "pointer",
                fontWeight: 600,
                color: theme.colors.blue[2],
                ":hover": {
                  color: theme.colors.blue[1],
                },
              })}
            >
              API Reference
            </Text>
            .
          </Text>
        </Stack> */}
      </Group>
    </Container>
  );
};

export default Page;
