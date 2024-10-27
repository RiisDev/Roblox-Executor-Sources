import { Avatar, Card, Group, Stack, Text } from "@mantine/core";
import { Rating } from "components/Rating";
import { FC } from "react";

type Props = {
  rating: number;
  username: string;
  review: string;
};

export const ReviewCard: FC<Props> = ({
  rating = 2,
  username = "aspect",
  review = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
}) => (
  <Card
    withBorder
    sx={{
      opacity: 0.5,
    }}
  >
    <Stack
      sx={{
        maxWidth: 400,
      }}
    >
      <Rating rating={rating} />
      <Group spacing="xs">
        <Avatar color="blue" radius="xl" size="sm">
          {username.slice(0, 2).toUpperCase()}
        </Avatar>
        <Text>{username}</Text>
      </Group>
      <Text color="dimmed" mt={-5}>
        {review}
      </Text>
    </Stack>
  </Card>
);
